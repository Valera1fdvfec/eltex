#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

// Объявление глобальной переменной
extern int sockfd;

// Обработчик сигналов
void signal_handler(int sig) {
    printf("[SERVER] Received signal %d, shutting down...\n", sig);
    close(sockfd);
    exit(0);
}

// Основная функция сервера
int main(void) {
    if (init_server() < 0) {
        exit(1);
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGKILL, signal_handler);

    while (1) {
        unsigned char buffer[MAX_BUFFER_SIZE];
        struct sockaddr_in src_addr;
        socklen_t addrlen = sizeof(src_addr);

        ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&src_addr, &addrlen);
        if (len < 0) {
            perror("recvfrom");
            continue;
        }

        process_packet(buffer, len, &src_addr);
    }

    close(sockfd);
    return 0;
}
