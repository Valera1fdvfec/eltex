#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 2010

typedef struct {
    int sock;
    struct sockaddr_in server_addr;
} Server;

Server server;

void cleanup() {
    printf("\nЗакрытие сервера и освобождение ресурсов...\n");
    if (server.sock > 0) {
        close(server.sock);
    }
    printf("Сервер завершил работу.\n");
    exit(0);
}

void signal_handler(int sig) {
    if (sig == SIGINT) {
        cleanup();
    }
}

void error(const char *msg) {
    perror(msg);
    cleanup();
}

void initialize_server(Server *server, int port) {
    server->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sock < 0) {
        error("Ошибка: не удалось открыть сокет");
    }

    int opt = 1;
    if (setsockopt(server->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 ||
        setsockopt(server->sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        error("Ошибка: не удалось установить опции сокета");
    }

    bzero((char *) &server->server_addr, sizeof(server->server_addr));
    server->server_addr.sin_family = AF_INET;
    server->server_addr.sin_addr.s_addr = INADDR_ANY;
    server->server_addr.sin_port = htons(port);

    if (bind(server->sock, (struct sockaddr *) &server->server_addr, sizeof(server->server_addr)) < 0) {
        error("Ошибка: не удалось привязать сокет");
    }
    listen(server->sock, 5);
}

void handle_client(int client_sock, fd_set *master_set) {
    char buffer[BUFFER_SIZE];
    int n;

    bzero(buffer, BUFFER_SIZE);
    n = read(client_sock, buffer, BUFFER_SIZE - 1);
    if (n <= 0) {
        if (n < 0) perror("Ошибка чтения данных");
        printf("Клиент отключен.\n");
        close(client_sock);
        FD_CLR(client_sock, master_set); // Удаление сокета из master_set
        return;
    }

    printf("Клиент отправил: %s\n", buffer);

    char operation;
    float num1, num2;
    if (sscanf(buffer, "%f %c %f", &num1, &operation, &num2) != 3) {
        snprintf(buffer, BUFFER_SIZE, "Ошибка: неверный формат данных\n");
        write(client_sock, buffer, strlen(buffer));
        return;
    }

    float result = 0.0;
    switch (operation) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if (num2 != 0) result = num1 / num2;
            else {
                snprintf(buffer, BUFFER_SIZE, "Ошибка: деление на ноль\n");
                write(client_sock, buffer, strlen(buffer));
                return;
            }
            break;
        default:
            snprintf(buffer, BUFFER_SIZE, "Ошибка: неизвестная операция\n");
            write(client_sock, buffer, strlen(buffer));
            return;
    }

    snprintf(buffer, BUFFER_SIZE, "Результат: %.2f\n", result);
    printf("Сервер отправил: %s\n", buffer);
    write(client_sock, buffer, strlen(buffer));
}

int main(int argc, char *argv[]) {
    int port = (argc < 2) ? DEFAULT_PORT : atoi(argv[1]);
    printf("Запуск сервера на порту %d\n", port);

    signal(SIGINT, signal_handler); // Установка обработчика сигнала SIGINT
    initialize_server(&server, port);

    fd_set master_set, read_set;
    int max_fd = server.sock;

    FD_ZERO(&master_set);
    FD_SET(server.sock, &master_set);

    while (1) {
        read_set = master_set;

        int activity = select(max_fd + 1, &read_set, NULL, NULL, NULL);
        if (activity < 0) {
            perror("Ошибка select");
            continue;
        }

	for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_set)) {
                if (i == server.sock) {
                    // Новое подключение
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_sock = accept(server.sock, (struct sockaddr *)&client_addr, &client_len);
                    if (client_sock < 0) {
                        perror("Ошибка accept");
                        continue;
                    }

                    printf("Новое подключение: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    FD_SET(client_sock, &master_set);
                    if (client_sock > max_fd) max_fd = client_sock;
                } else {
                    // Обработка данных от клиента
                    handle_client(i, &master_set);
                }
            }
        }
    }

    cleanup();
    return 0;
}
