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
        // Выводим меню
        printf("\nВыберите действие:\n");
        printf("1. Ввести математическое выражение (например, 5 + 3)\n");
        printf("2. Отправить файл на сервер\n");
        printf("0. Выйти\n");
        printf("Ваш выбор: ");
        
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE - 1, stdin);
        
        if (strncmp(buffer, "0", 1) == 0) {
            printf("Завершаем соединение...\n");
            break;
        }

        if (strncmp(buffer, "1", 1) == 0) {
            // Ввод математического выражения
            printf("Введите математическое выражение (например, 5 + 3): ");
            bzero(buffer, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE - 1, stdin);

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

        } else if (strncmp(buffer, "2", 1) == 0) {
            // Отправка файла на сервер
            printf("Введите путь к файлу для отправки: ");
            bzero(buffer, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE - 1, stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Убираем лишний символ новой строки

            FILE *file = fopen(buffer, "rb");
            if (file == NULL) {
                printf("Ошибка: не удалось открыть файл\n");
                continue;
            }

            // Отправляем команду на сервер о передаче файла
            write(client->sock, "FILE", 4);
            
            // Отправка размера файла
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            rewind(file);
            write(client->sock, &file_size, sizeof(file_size));

            // Чтение файла и отправка данных
            char *file_buffer = malloc(file_size);
            fread(file_buffer, 1, file_size, file);

            // Убираем нулевые байты и слэши
            for (int i = 0; i < file_size; ++i) {
                if (file_buffer[i] == '\0' || file_buffer[i] == '/') {
                    file_buffer[i] = ' ';
                }
            }

            write(client->sock, file_buffer, file_size);

            // Ожидаем ответа от сервера
            bzero(buffer, BUFFER_SIZE);
            n = read(client->sock, buffer, BUFFER_SIZE - 1);
            if (n < 0) {
                error("Ошибка: не удалось получить данные");
            }
            printf("Ответ от сервера: %s\n", buffer);
            
            free(file_buffer);
            fclose(file);
        } else {
            printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }

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
