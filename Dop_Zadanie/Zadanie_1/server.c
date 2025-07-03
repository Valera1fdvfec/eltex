#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>

#define MAX_CLIENTS 100

// Глобальная переменная
int sockfd;

// Структура для хранения информации о клиенте
struct client_info {
    struct in_addr ip;
    uint16_t port;
    int msg_count;
    int active;
    int client_number; // Уникальный номер клиента
};

// Массив для хранения информации о клиентах
static struct client_info clients[MAX_CLIENTS];

// Статическая переменная для номеров
static int next_client_number = 1;

// Поиск клиента по IP и порту
int find_client(struct in_addr ip, uint16_t port) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active &&
            clients[i].ip.s_addr == ip.s_addr &&
            clients[i].port == port) {
            return i;
        }
    }
    return -1;
}

// Добавление или переподключение клиента с сохранением номера
int add_client(struct in_addr ip, uint16_t port) {
    int existing_idx = find_client(ip, port);
    if (existing_idx != -1) {
        clients[existing_idx].msg_count = 0; // Сбрасываем счетчик сообщений
        clients[existing_idx].active = 1;
        printf("[SERVER] Client reconnected: %s:%d (Клиент %d)\n", inet_ntoa(ip), ntohs(port), clients[existing_idx].client_number);
        return existing_idx;
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].ip = ip;
            clients[i].port = port;
            clients[i].msg_count = 0;
            clients[i].active = 1;
            clients[i].client_number = next_client_number++; 
            printf("[SERVER] New client connected: %s:%d (Клиент %d)\n", inet_ntoa(ip), ntohs(port), clients[i].client_number);
            return i;
        }
    }
    return -1;
}

// Удаление клиента
void remove_client(int idx) {
    if (clients[idx].active) {
        printf("[SERVER] Client disconnected: %s:%d (Клиент %d)\n", inet_ntoa(clients[idx].ip), ntohs(clients[idx].port), clients[idx].client_number);
        clients[idx].active = 0;
        clients[idx].msg_count = 0;
    }
}

// Функция обработки пакета
void process_packet(unsigned char *buffer, ssize_t len, struct sockaddr_in *src_addr) {
    struct ip_header *ip = (struct ip_header *)buffer;
    if (ip->protocol != IPPROTO_UDP) return;

    int iphdrlen = ip->ihl * 4;
    struct udp_header *udp = (struct udp_header *)(buffer + iphdrlen);
    if (ntohs(udp->dest) != SERVER_PORT) return;

    struct in_addr client_ip;
    client_ip.s_addr = ip->saddr;
    uint16_t client_port = udp->source;

    int client_idx = find_client(client_ip, client_port);
    if (client_idx == -1) {
        client_idx = add_client(client_ip, client_port);
        if (client_idx == -1) {
            printf("[SERVER] Max clients reached, dropping packet from %s:%d\n", inet_ntoa(client_ip), ntohs(client_port));
            return;
        }
    }

    int udp_data_len = ntohs(udp->len) - sizeof(struct udp_header);
    char *data = (char *)(buffer + iphdrlen + sizeof(struct udp_header));

    printf("[SERVER] Received from %s:%d (Клиент %d) — \"%.*s\"\n", inet_ntoa(client_ip), ntohs(client_port), clients[client_idx].client_number, udp_data_len, data);

    if (udp_data_len == 4 && strncmp(data, "exit", 4) == 0) {
        remove_client(client_idx);
        return;
    }

    clients[client_idx].msg_count++;

    char response[1024];
    int resp_len = snprintf(response, sizeof(response), "%.*s %d", udp_data_len, data, clients[client_idx].msg_count);

    unsigned char sendbuf[1500];
    memset(sendbuf, 0, sizeof(sendbuf));

    struct ip_header *rip = (struct ip_header *)sendbuf;
    struct udp_header *rudp = (struct udp_header *)(sendbuf + sizeof(struct ip_header));
    char *rdata = (char *)(sendbuf + sizeof(struct ip_header) + sizeof(struct udp_header));
    memcpy(rdata, response, resp_len);

    rip->version = 4;
    rip->ihl = 5;
    rip->tos = 0;
    rip->tot_len = htons(sizeof(struct ip_header) + sizeof(struct udp_header) + resp_len);
    rip->id = htons(54321);
    rip->frag_off = 0;
    rip->ttl = 64;
    rip->protocol = IPPROTO_UDP;
    rip->saddr = ip->daddr;
    rip->daddr = client_ip.s_addr;
    rip->check = 0;
    rip->check = checksum((unsigned short *)rip, sizeof(struct ip_header)/2);

    rudp->source = htons(SERVER_PORT);
    rudp->dest = client_port;
    rudp->len = htons(sizeof(struct udp_header) + resp_len);
    rudp->check = 0;
    rudp->check = udp_checksum(rip, rudp, (unsigned char *)rdata, resp_len);

    struct sockaddr_in to_addr;
    to_addr.sin_family = AF_INET;
    to_addr.sin_port = client_port;
    to_addr.sin_addr = client_ip;

    ssize_t sent = sendto(sockfd, sendbuf, sizeof(struct ip_header) + sizeof(struct udp_header) + resp_len,
                          0, (struct sockaddr *)&to_addr, sizeof(to_addr));
    if (sent < 0)
        perror("sendto");
    else
        printf("[SERVER] Sent response to %s:%d (Клиент %d) — \"%.*s\"\n", inet_ntoa(client_ip), ntohs(client_port), clients[client_idx].client_number, resp_len, response);
}

// Функция инициализации сервера
int init_server(void) {
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    int one = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    // Привязываем сокет к 0.0.0.0 для приема пакетов со всех интерфейсов
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    printf("[SERVER] Server started. Listening on 0.0.0.0:%d...\n", SERVER_PORT);
    return 0;
}
