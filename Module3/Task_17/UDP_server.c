#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 2010
#define BUF_SIZE 1024

int server_sock;
struct sockaddr_in server_addr;
struct sockaddr_in client_addrs[10];
int client_count = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(const char *message, struct sockaddr_in sender_addr) {
    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++) {
        if (client_addrs[i].sin_addr.s_addr != sender_addr.sin_addr.s_addr || client_addrs[i].sin_port != sender_addr.sin_port) {
            if (sendto(server_sock, message, strlen(message), 0, (struct sockaddr *)&client_addrs[i], sizeof(client_addrs[i])) < 0) {
                perror("Ошибка при отправке сообщения клиенту");
            }
        }
    }
    pthread_mutex_unlock(&client_mutex);
}

void *handle_client(void *arg) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int received = recvfrom(server_sock, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (received < 0) {
            perror("Ошибка при получении сообщения");
            exit(EXIT_FAILURE);
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(client_addr.sin_port);
        printf("Получено сообщение от клиента %s:%d:\n%s\n", client_ip, client_port, buffer);

        pthread_mutex_lock(&client_mutex);
        int is_new_client = 1;
        for (int i = 0; i < client_count; i++) {
            if (client_addrs[i].sin_addr.s_addr == client_addr.sin_addr.s_addr && client_addrs[i].sin_port == client_addr.sin_port) {
                is_new_client = 0;
                break;
            }
        }
        if (is_new_client) {
            client_addrs[client_count++] = client_addr;
            printf("Новый клиент, IP: %s:%d;\nТекущее количество клиентов: %d\n", client_ip, client_port, client_count);
        }
        pthread_mutex_unlock(&client_mutex);

        if (strcmp(buffer, "/exit") == 0) {
            pthread_mutex_lock(&client_mutex);
            for (int i = 0; i < client_count; i++) {
                if (client_addrs[i].sin_addr.s_addr == client_addr.sin_addr.s_addr && client_addrs[i].sin_port == client_addr.sin_port) {
                    for (int j = i; j < client_count - 1; j++) {
                        client_addrs[j] = client_addrs[j + 1];
                    }
                    client_count--;

                    printf("Клиент с IP: %s:%d покинул чат. Текущее количество клиентов: %d\n", client_ip, client_port, client_count);

                    char leave_message[BUF_SIZE + 50];
                    snprintf(leave_message, sizeof(leave_message), "Клиент %s:%d покинул чат. Текущее количество клиентов: %d", client_ip, client_port, client_count);
                    broadcast_message(leave_message, client_addr);

                    break;
                }
            }
            pthread_mutex_unlock(&client_mutex);
            continue;
        }

        char broadcast_msg[BUF_SIZE + 50];
        snprintf(broadcast_msg, sizeof(broadcast_msg), "%s:%d: %s", client_ip, client_port, buffer);
        broadcast_message(broadcast_msg, client_addr);
    }
    return NULL;
}

int main() {
    // Создание сокета
    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязка сокета к адресу
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Получение и вывод IP-адреса сервера
    char server_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, INET_ADDRSTRLEN);
    printf("Сервер запущен на IP: %s, порт: %d\n", server_ip, PORT);
    printf("Сервер готов принимать сообщения...\n");

    // Создание потока для обработки клиентов
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, handle_client, NULL) != 0) {
        perror("Ошибка при создании потока");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Ожидание завершения потока
    pthread_join(thread_id, NULL);

    close(server_sock);
    return 0;
}
