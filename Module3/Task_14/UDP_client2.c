#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 2010
#define BUF_SIZE 1024

int client_sock;
struct sockaddr_in server_addr;
char server_ip[20];
char client_name[50];

void *receive_messages(void *arg) {
    char buffer[BUF_SIZE];
    struct sockaddr_in from_addr;
    socklen_t from_addr_len = sizeof(from_addr);
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int received = recvfrom(client_sock, buffer, BUF_SIZE, 0, (struct sockaddr *)&from_addr, &from_addr_len);
        if (received < 0) {
            perror("Ошибка при получении сообщения");
            exit(EXIT_FAILURE);
        }
        printf("%s\n> ", buffer);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    // Создание сокета
    client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Ввод IP-адреса сервера
    printf("Введите IP-адрес сервера: ");
    scanf("%s", server_ip);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Неверный IP-адрес");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    // Ввод имени клиента
    printf("Введите ваше имя: ");
    scanf("%s", client_name);

    // Создание потока для получения сообщений
    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_messages, NULL) != 0) {
        perror("Ошибка при создании потока");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    // Отправка имени клиента серверу
    if (sendto(client_sock, client_name, strlen(client_name), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при отправке имени клиента");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    // Основной цикл отправки сообщений
    char message[BUF_SIZE];
    while (1) {
        printf("> ");
        fgets(message, BUF_SIZE, stdin);
        message[strcspn(message, "\n")] = 0; // Удаляем символ новой строки

        if (strcmp(message, "/exit") == 0) {
            break;
        }

        char formatted_message[BUF_SIZE + 50];
        snprintf(formatted_message, sizeof(formatted_message), "%.100s: %.100s", client_name, message);

        // Отправка сообщения серверу
        if (sendto(client_sock, formatted_message, strlen(formatted_message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Ошибка при отправке сообщения");
            close(client_sock);
            exit(EXIT_FAILURE);
        }
    }

    // Завершение работы
    close(client_sock);
    return 0;
}
