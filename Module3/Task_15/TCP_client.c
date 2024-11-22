#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

typedef struct {
    int sock;
    struct sockaddr_in server_addr;
} Client;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void initialize_client(Client *client, const char *hostname, int port) {
    struct hostent *server;
    
    client->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client->sock < 0) {
        error("Ошибка: не удалось открыть сокет");
    }
    
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "Ошибка: сервер не найден\n");
        exit(1);
    }
    
    bzero((char *) &client->server_addr, sizeof(client->server_addr));
    client->server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&client->server_addr.sin_addr.s_addr, server->h_length);
    client->server_addr.sin_port = htons(port);
}

void communicate(Client *client) {
    char buffer[BUFFER_SIZE];
    int n;

    if (connect(client->sock, (struct sockaddr *) &client->server_addr, sizeof(client->server_addr)) < 0) {
        error("Ошибка: не удалось подключиться");
    }

    while (1) {
        printf("Введите число, операцию и второе число (если хотите выйти, введите '0'): ");
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE - 1, stdin);

        // Проверка на команду выхода
        if (strncmp(buffer, "0", 1) == 0) {
            printf("Завершаем соединение...\n");
            break;
        }

        n = write(client->sock, buffer, strlen(buffer));
        if (n < 0) {
            error("Ошибка: не удалось отправить данные");
        }

        bzero(buffer, BUFFER_SIZE);
        n = read(client->sock, buffer, BUFFER_SIZE - 1);
        if (n < 0) {
            error("Ошибка: не удалось получить данные");
        }
        printf("Результат: %s\n", buffer);
    }

    // Закрываем соединение после завершения работы клиента
    close(client->sock);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s хост порт\n", argv[0]);
        exit(1);
    }

    Client client;
    initialize_client(&client, argv[1], atoi(argv[2]));
    communicate(&client);
    return 0;
}
