#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

void handle_client(int newsockfd, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    int n;

    printf("Подключился клиент: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        n = read(newsockfd, buffer, BUFFER_SIZE - 1);
        if (n < 0) {
            perror("Ошибка: не удалось получить данные от клиента");
            break;
        }

        if (n == 0) {  // Клиент закрыл соединение
            printf("Клиент закрыл соединение.\n");
            break;
        }

        // Если клиент выбрал отправку файла
        if (strncmp(buffer, "FILE", 4) == 0) {
            FILE *file = fopen("new_file.txt", "wb");
            if (file == NULL) {
                snprintf(buffer, BUFFER_SIZE, "Ошибка при создании файла\n");
                write(newsockfd, buffer, strlen(buffer));
                continue;
            }

            int file_size;
            read(newsockfd, &file_size, sizeof(file_size)); // Читаем размер файла
            char *file_buffer = malloc(file_size);

            if (file_buffer == NULL) {
                snprintf(buffer, BUFFER_SIZE, "Ошибка выделения памяти\n");
                write(newsockfd, buffer, strlen(buffer));
                continue;
            }

            read(newsockfd, file_buffer, file_size);

            // Убираем нулевые байты и слэши из данных
            for (int i = 0; i < file_size; ++i) {
                if (file_buffer[i] == '\0' || file_buffer[i] == '/') {
                    file_buffer[i] = ' ';
                }
            }

            fwrite(file_buffer, 1, file_size, file);
            fclose(file);

            // Логируем в консоль
            printf("Файл успешно получен от клиента: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            snprintf(buffer, BUFFER_SIZE, "Файл успешно получен и сохранен\n");
            write(newsockfd, buffer, strlen(buffer));
            free(file_buffer);
            continue;  // После получения файла продолжаем вычисления
        }

        // Математические операции
        char operation;
        float num1, num2;
        if (sscanf(buffer, "%f %c %f", &num1, &operation, &num2) != 3) {
            snprintf(buffer, BUFFER_SIZE, "Ошибка: неверный формат данных\n");
            write(newsockfd, buffer, strlen(buffer));
            continue;
        }

        float result = 0.0;
        switch (operation) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 != 0) {
                    result = num1 / num2;
                } else {
                    snprintf(buffer, BUFFER_SIZE, "Ошибка: деление на ноль\n");
                    write(newsockfd, buffer, strlen(buffer));
                    continue;
                }
                break;
            default:
                snprintf(buffer, BUFFER_SIZE, "Ошибка: неизвестная операция\n");
                write(newsockfd, buffer, strlen(buffer));
                continue;
        }

        snprintf(buffer, BUFFER_SIZE, "Результат: %.2f\n", result);
        printf("Сервер отправил: %s\n", buffer);
        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Ошибка: не удалось отправить данные клиенту");
        }
    }

    close(newsockfd);
    printf("Соединение с клиентом закрыто\n");
}

int main(int argc, char *argv[]) {
    int port = (argc < 2) ? DEFAULT_PORT : atoi(argv[1]);
    printf("Запуск сервера на порту %d\n", port);

    signal(SIGINT, signal_handler); // Установка обработчика сигнала SIGINT
    initialize_server(&server, port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int newsockfd = accept(server.sock, (struct sockaddr *) &client_addr, &client_len);
        if (newsockfd < 0) {
            perror("Ошибка: не удалось принять подключение");
            continue;
        }
        handle_client(newsockfd, client_addr);
    }

    cleanup();
    return 0;
}
