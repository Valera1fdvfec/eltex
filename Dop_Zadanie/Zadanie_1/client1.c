#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <signal.h>

// Глобальные переменные для сокета и адреса сервера
static int sockfd = -1; // Дескриптор сокета
static struct sockaddr_in dest; // Адрес сервера

// Функция отправки сообщения "exit" серверу при завершении
void send_exit_message(void) {
    if (sockfd < 0) return; // Проверка, открыт ли сокет

    unsigned char packet[4096]; // Буфер для пакета
    struct ip_header *ip = (struct ip_header *)packet; // IP-заголовок
    struct udp_header *udp = (struct udp_header *)(packet + sizeof(struct ip_header)); // UDP-заголовок
    char *data = (char *)(packet + sizeof(struct ip_header) + sizeof(struct udp_header)); // Данные пакета
    const char *message = "exit"; // Сообщение для отключения
    int payload_len = strlen(message);

    memcpy(data, message, payload_len); // Копируем "exit" в пакет

    // Заполняем IP-заголовок
    ip->version = 4;
    ip->ihl = 5;
    ip->tos = 0;
    ip->tot_len = htons(sizeof(struct ip_header) + sizeof(struct udp_header) + payload_len);
    ip->id = htons(12345);
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_UDP;
    ip->saddr = inet_addr("127.0.0.1"); // Локальный IP
    ip->daddr = dest.sin_addr.s_addr; // IP сервера
    ip->check = 0;
    ip->check = checksum((unsigned short *)ip, sizeof(struct ip_header)/2); // Вычисляем контрольную сумму IP

    // Заполняем UDP-заголовок
    udp->source = htons(CLIENT_PORT); // Порт клиента
    udp->dest = htons(SERVER_PORT); // Порт сервера
    udp->len = htons(sizeof(struct udp_header) + payload_len);
    udp->check = 0;
    udp->check = udp_checksum(ip, udp, (unsigned char *)data, payload_len); // Контрольная сумма UDP

    // Настраиваем адрес для отправки
    struct sockaddr_in to_addr;
    to_addr.sin_family = AF_INET;
    to_addr.sin_port = htons(SERVER_PORT);
    to_addr.sin_addr = dest.sin_addr;

    // Отправляем пакет
    ssize_t sent = sendto(sockfd, packet, sizeof(struct ip_header) + sizeof(struct udp_header) + payload_len,
                          0, (struct sockaddr *)&to_addr, sizeof(to_addr));
    if (sent < 0) {
        perror("sendto exit message");
    } else {
        printf("Sent exit message to server\n");
    }
}

// Обработчик сигнала Ctrl+C
void signal_handler(int sig) {
    send_exit_message(); // Отправляем "exit"
    close(sockfd); // Закрываем сокет
    exit(0); // Завершаем программу
}

int main() {
    // Создаем сырой сокет для UDP
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Включаем ручное заполнение IP-заголовка
    int one = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        close(sockfd);
        exit(1);
    }

    // Настраиваем адрес сервера
    dest.sin_family = AF_INET;
    dest.sin_port = htons(SERVER_PORT);
    inet_aton("127.0.0.1", &dest.sin_addr);

    // Регистрируем обработчики для завершения программы
    atexit(send_exit_message); // При нормальном завершении
    signal(SIGINT, signal_handler); // При Ctrl+C

    char input[1024]; // Буфер для ввода сообщения
    unsigned char packet[4096]; // Буфер для отправляемого пакета

    while (1) {
        printf("Enter message: ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break; // Читаем ввод

        input[strcspn(input, "\n")] = 0; // Удаляем '\n'
        int payload_len = strlen(input);
        if (payload_len == 0) continue; // Пропускаем пустой ввод

        // Формируем пакет
        struct ip_header *ip = (struct ip_header *)packet;
        struct udp_header *udp = (struct udp_header *)(packet + sizeof(struct ip_header));
        char *data = (char *)(packet + sizeof(struct ip_header) + sizeof(struct udp_header));

        memcpy(data, input, payload_len); // Копируем сообщение в пакет

        // Заполняем IP-заголовок
        ip->version = 4;
        ip->ihl = 5;
        ip->tos = 0;
        ip->tot_len = htons(sizeof(struct ip_header) + sizeof(struct udp_header) + payload_len);
        ip->id = htons(12345);
        ip->frag_off = 0;
        ip->ttl = 64;
        ip->protocol = IPPROTO_UDP;
        ip->saddr = inet_addr("127.0.0.1");
        ip->daddr = dest.sin_addr.s_addr;
        ip->check = 0;
        ip->check = checksum((unsigned short *)ip, sizeof(struct ip_header)/2);

        // Заполняем UDP-заголовок
        udp->source = htons(CLIENT_PORT);
        udp->dest = htons(SERVER_PORT);
        udp->len = htons(sizeof(struct udp_header) + payload_len);
        udp->check = 0;
        udp->check = udp_checksum(ip, udp, (unsigned char *)data, payload_len);

        // Настраиваем адрес для отправки
        struct sockaddr_in to_addr;
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons(SERVER_PORT);
        to_addr.sin_addr = dest.sin_addr;

        // Отправляем пакет
        ssize_t sent = sendto(sockfd, packet, sizeof(struct ip_header) + sizeof(struct udp_header) + payload_len,
                              0, (struct sockaddr *)&to_addr, sizeof(to_addr));
        if (sent < 0) {
            perror("sendto");
            continue;
        }
        printf("Message sent: %s\n", input);

        // Ожидаем ответа от сервера
        while (1) {
            unsigned char buffer[4096]; // Буфер для ответа
            struct sockaddr_in from_addr;
            socklen_t from_len = sizeof(from_addr);

            ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&from_addr, &from_len);
            if (len < 0) {
                perror("recvfrom");
                continue;
            }

            struct ip_header *rip = (struct ip_header *)buffer;
            if (rip->protocol != IPPROTO_UDP) continue; // Пропускаем не-UDP пакеты

            int iphdrlen = rip->ihl * 4;
            struct udp_header *rupd = (struct udp_header *)(buffer + iphdrlen);

            // Проверяем, что ответ от сервера и на наш порт
            if (rupd->source != htons(SERVER_PORT) || rupd->dest != htons(CLIENT_PORT))
                continue;

            int datalen = ntohs(rupd->len) - sizeof(struct udp_header);
            if (datalen <= 0) continue;

            char *rdata = (char *)(buffer + iphdrlen + sizeof(struct udp_header));

            // Копируем ответ с добавлением '\0'
            char safe[1024];
            if (datalen > 1023) datalen = 1023;
            memcpy(safe, rdata, datalen);
            safe[datalen] = '\0';

            printf("Received response: %s\n", safe);
            break;
        }
    }

    send_exit_message(); 
    close(sockfd); // Закрываем сокет
    return 0;
}
