#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

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

void display_local_ip() {
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("Ошибка: не удалось получить IP-адреса");
        return;
    }

    printf("Сервер запущен на следующих IP-адресах:\n");
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            void *addr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, addr, ip, INET_ADDRSTRLEN);
            printf("- %s\n", ip);
        }
    }

    freeifaddrs(ifaddr);
}


void initialize_server(Server *server, int port) {
    server->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sock < 0) {
        error("Ошибка: не удалось открыть сокет");
    }

    int opt = 1;
    if (setsockopt(server->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        error("Ошибка: не удалось установить опции сокета");
    }
    
    bzero((char *) &server->server_addr, sizeof(server->server_addr));
    server->server_addr.sin_family = AF_INET;
    server->server_addr.sin_addr.s_addr = INADDR_ANY;
    server->server_addr.sin_port = htons(port);
    
    if (bind(server->sock, (struct sockaddr *) &server->server_addr, sizeof(server->server_addr)) < 0) {
        error("Ошибка: не удалось привязать сокет");
    }
    display_local_ip();
    printf("Порт: %d\n", port);

    listen(server->sock,5); 
}

void handle_file_transfer(int newsockfd) {
    char buffer[BUFFER_SIZE];
    char filename[BUFFER_SIZE];
    FILE *file;

    // Получение имени файла
    bzero(filename, BUFFER_SIZE);
    if (read(newsockfd, filename, BUFFER_SIZE - 1) <= 0) {
        perror("Ошибка: не удалось получить имя файла");
        return;
    }

    printf("Получен файл: %s\n", filename);
    file = fopen(filename, "wb");
    if (!file) {
        perror("Ошибка: не удалось создать файл");
        return;
    }

    // Чтение данных файла
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        int bytes_read = read(newsockfd, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            break;
        }
        fwrite(buffer, 1, bytes_read, file);
    }

    printf("Передача файла завершена.\n");
    fclose(file);
    snprintf(buffer, BUFFER_SIZE, "Файл считан успешно\n");
    write(newsockfd, buffer, strlen(buffer));
}

void handle_calculation(int newsockfd, char *buffer) {
    char operation;
    float num1, num2, result;

    if (sscanf(buffer, "%f %c %f", &num1, &operation, &num2) != 3) {
        snprintf(buffer, BUFFER_SIZE, "Ошибка: неверный формат данных. Используйте: <число> <операция> <число>\n");
        write(newsockfd, buffer, strlen(buffer));
        return;
    }
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
            if (num2 == 0) {
                snprintf(buffer, BUFFER_SIZE, "Ошибка: деление на 0\n");
                write(newsockfd, buffer, strlen(buffer));
                return;
            }
            result = num1 / num2;
            break;
        default:
            snprintf(buffer, BUFFER_SIZE, "Ошибка: неизвестная операция '%c'\n", operation);
            write(newsockfd, buffer, strlen(buffer));
            return;
    }

    snprintf(buffer, BUFFER_SIZE, "Результат: %.2f\n", result);
    write(newsockfd, buffer, strlen(buffer));
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

        if (n == 0) {
            printf("Клиент закрыл соединение.\n");
            break;
        }

        printf("Клиент отправил: %s\n", buffer);

        if (strncmp(buffer, "FILE", 4) == 0) {
            handle_file_transfer(newsockfd);
            break;
        }

	handle_calculation(newsockfd, buffer);
    }

    close(newsockfd);
    printf("Соединение с клиентом закрыто\n");
}

int main(int argc, char *argv[]) {
    int port = (argc < 2) ? DEFAULT_PORT : atoi(argv[1]);

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
