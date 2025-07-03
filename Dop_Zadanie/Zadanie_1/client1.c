#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>

// Глобальные переменные для сокета, адреса сервера и порта клиента
int sockfd = -1; // Дескриптор сокета
struct sockaddr_in dest; // Адрес сервера
int client_port; // Порт клиента

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
    ip->saddr = INADDR_ANY; 
    ip->daddr = dest.sin_addr.s_addr; // IP сервера
    ip->check = 0;
    ip->check = checksum((unsigned short *)ip, sizeof(struct ip_header)/2); // Вычисляем контрольную сумму IP

    // Заполняем UDP-заголовок
    udp->source = htons(client_port); // Порт клиента из аргумента
    udp->dest = dest.sin_port; // Порт сервера
    udp->len = htons(sizeof(struct udp_header) + payload_len);
    udp->check = 0;
    udp->check = udp_checksum(ip, udp, (unsigned char *)data, payload_len); // Контрольная сумма UDP

    // Настраиваем адрес для отправки
    struct sockaddr_in to_addr;
    to_addr.sin_family = AF_INET;
    to_addr.sin_port = dest.sin_port;
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

// Функция для отправки сообщения серверу
int send_message(const char *input, int payload_len) {
    unsigned char packet[4096]; // Буфер для отправляемого пакета
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
    ip->saddr = INADDR_ANY; // Ядро выберет локальный IP
    ip->daddr = dest.sin_addr.s_addr; // IP сервера
    ip->check = 0;
    ip->check = checksum((unsigned short *)ip, sizeof(struct ip_header)/2);

    // Заполняем UDP-заголовок
    udp->source = htons(client_port); // Порт клиента из аргумента
    udp->dest = dest.sin_port; // Порт сервера
    udp->len = htons(sizeof(struct udp_header) + payload_len);
    udp->check = 0;
    udp->check = udp_checksum(ip, udp, (unsigned char *)data, payload_len);

    // Настраиваем адрес для отправки
    struct sockaddr_in to_addr;
    to_addr.sin_family = AF_INET;
    to_addr.sin_port = dest.sin_port;
    to_addr.sin_addr = dest.sin_addr;

    // Отправляем пакет
    ssize_t sent = sendto(sockfd, packet, sizeof(struct ip_header) + sizeof(struct udp_header) + payload_len,
                          0, (struct sockaddr *)&to_addr, sizeof(to_addr));
    if (sent < 0) {
        perror("sendto");
        return -1;
    }
    printf("Message sent: %s\n", input);
    return 0;
}

// Функция для получения ответа от сервера
int receive_response(void) {
    unsigned char buffer[4096]; // Буфер для ответа
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    while (1) {
        ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&from_addr, &from_len);
        if (len < 0) {
            perror("recvfrom");
            return -1;
        }

        struct ip_header *rip = (struct ip_header *)buffer;
        if (rip->protocol != IPPROTO_UDP) continue; // Пропускаем не-UDP пакеты

        int iphdrlen = rip->ihl * 4;
        struct udp_header *rupd = (struct udp_header *)(buffer + iphdrlen);

        // Проверяем, что ответ от сервера и на наш порт
        if (rupd->source != dest.sin_port || rupd->dest != htons(client_port))
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
        return 0;
    }
}

// Функция инициализации сокета и адреса сервера
int init_client(const char *server_ip, int client_port_arg) {
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    // Включаем ручное заполнение IP-заголовка
    int one = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    // Настраиваем адрес сервера
    dest.sin_family = AF_INET;
    dest.sin_port = htons(SERVER_PORT); // Фиксированный порт сервера
    if (inet_aton(server_ip, &dest.sin_addr) == 0) {
        fprintf(stderr, "Invalid server IP address: %s\n", server_ip);
        close(sockfd);
        return -1;
    }

    // Сохраняем порт клиента
    client_port = client_port_arg;

    return 0;
}
