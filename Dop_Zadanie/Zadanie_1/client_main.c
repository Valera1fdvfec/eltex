#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// Объявление глобальных переменных
extern int sockfd;

// Обработчик сигналов
void signal_handler(int sig) {
    printf("[CLIENT] Received signal %d, shutting down...\n", sig);
    send_exit_message();
    close(sockfd);
    exit(0);
}

// Основная функция клиента
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <client_port>\n", argv[0]);
        exit(1);
    }

    // Инициализация клиента
    if (init_client(argv[1], atoi(argv[2])) < 0) {
        exit(1);
    }

    // Регистрируем обработчики сигналов
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGKILL, signal_handler); // Примечание: SIGKILL не может быть перехвачен, добавлено для полноты

    char input[1024]; // Буфер для ввода сообщения

    while (1) {
        printf("Enter message: ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break; // Читаем ввод

        input[strcspn(input, "\n")] = 0; // Удаляем '\n'
        int payload_len = strlen(input);
        if (payload_len == 0) continue; // Пропускаем пустой ввод

        if (send_message(input, payload_len) < 0) continue;

        if (receive_response() < 0) continue;
    }

    send_exit_message();
    close(sockfd);
    return 0;
}
