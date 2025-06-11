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

#define MAX_CLIENTS 2

// Структура для хранения информации о клиенте
struct client_info {
    struct in_addr ip;
    uint16_t port;
    int msg_count;
    int active;
};

static struct client_info clients[MAX_CLIENTS];
static int sockfd;

// Отправка уведомления всем активным клиентам, кроме exclude_idx
void send_notification(const char *message, int exclude_idx) {
    unsigned char sendbuf[1500];
    memset(sendbuf, 0, sizeof(sendbuf));

    int resp_len = strlen(message);
    struct ip_header *rip = (struct ip_header *)sendbuf;
    struct udp_header *rudp = (struct udp_header *)(sendbuf + sizeof(struct ip_header));
    char *rdata = (char *)(sendbuf + sizeof(struct ip_header) + sizeof(struct udp_header));
    memcpy(rdata, message, resp_len);

	// Заполняем IP-заголовок
    rip->version = 4;
    rip->ihl = 5;
    rip->tos = 0;
    rip->tot_len = htons(sizeof(struct ip_header) + sizeof(struct udp_header) + resp_len);
    rip->id = htons(54321);
    rip->frag_off = 0;
    rip->ttl = 64;
    rip->protocol = IPPROTO_UDP;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && i != exclude_idx) {
            rip->saddr = inet_addr("127.0.0.1");
            rip->daddr = clients[i].ip.s_addr;

            rip->check = 0;
            rip->check = checksum((unsigned short *)rip, sizeof(struct ip_header)/2);

			// Заполняем UDP-заголовок
            rudp->source = htons(SERVER_PORT);
            rudp->dest = clients[i].port;
            rudp->len = htons(sizeof(struct udp_header) + resp_len);
            rudp->check = 0;
            rudp->check = udp_checksum(rip, rudp, (unsigned char *)rdata, resp_len);
			
			// Настраиваем адрес для отправки
            struct sockaddr_in to_addr;
            to_addr.sin_family = AF_INET;
            to_addr.sin_port = clients[i].port;
            to_addr.sin_addr = clients[i].ip;

			// Отправляем уведомление
            ssize_t sent = sendto(sockfd, sendbuf, sizeof(struct ip_header) + sizeof(struct udp_header) + resp_len,
                                  0, (struct sockaddr *)&to_addr, sizeof(to_addr));
            if (sent < 0) {
                perror("sendto notification");
            } else {
                printf("[SERVER] Sent notification to %s:%d — \"%s\"\n", inet_ntoa(clients[i].ip), ntohs(clients[i].port), message);
            }
        }
    }
}

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

// Добавление или переподключение клиента
int add_client(struct in_addr ip, uint16_t port) {
    // Проверяем, есть ли клиент с таким IP и портом, и сбрасываем его данные
    int existing_idx = find_client(ip, port);
    if (existing_idx != -1) {
        clients[existing_idx].msg_count = 0; // Сбрасываем счетчик сообщений
        clients[existing_idx].active = 1;
        printf("[SERVER] Client reconnected: %s:%d (Клиент %d)\n", inet_ntoa(ip), ntohs(port), existing_idx + 1);
        char message[64];
        snprintf(message, sizeof(message), "Клиент %d подключился", existing_idx + 1);
        send_notification(message, existing_idx);
        return existing_idx;
    }

    // Ищем свободный слот
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].ip = ip;
            clients[i].port = port;
            clients[i].msg_count = 0; // Сбрасываем счетчик для нового клиента
            clients[i].active = 1;
            printf("[SERVER] New client connected: %s:%d (Клиент %d)\n", inet_ntoa(ip), ntohs(port), i + 1);
            char message[64];
            snprintf(message, sizeof(message), "Клиент %d подключился", i + 1);
            send_notification(message, i);
            return i;
        }
    }
    return -1;
}

// Удаление клиента
void remove_client(int idx) {
    if (clients[idx].active) {
        char message[64];
        snprintf(message, sizeof(message), "Клиент %d отключился", idx + 1);
        printf("[SERVER] Client disconnected: %s:%d (Клиент %d)\n", inet_ntoa(clients[idx].ip), ntohs(clients[idx].port), idx + 1);
        send_notification(message, idx);
        clients[idx].active = 0;
        clients[idx].msg_count = 0;
    }
}

int main() {
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

    printf("[SERVER] Server started. Listening on UDP port %d...\n", SERVER_PORT);

    while (1) {
        unsigned char buffer[MAX_BUFFER_SIZE];
        struct sockaddr_in src_addr;
        socklen_t addrlen = sizeof(src_addr);
		
		// Принимаем пакет
        ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&src_addr, &addrlen);
        if (len < 0) {
            perror("recvfrom");
            continue;
        }

        struct ip_header *ip = (struct ip_header *)buffer;

        if (ip->protocol != IPPROTO_UDP)
            continue;

        int iphdrlen = ip->ihl * 4;
        struct udp_header *udp = (struct udp_header *)(buffer + iphdrlen);

        if (ntohs(udp->dest) != SERVER_PORT)
            continue;

		// Извлекаем IP и порт клиента
        struct in_addr client_ip;
        client_ip.s_addr = ip->saddr;
        uint16_t client_port = udp->source;

        int client_idx = find_client(client_ip, client_port);
        if (client_idx == -1) {
            client_idx = add_client(client_ip, client_port);
            if (client_idx == -1) {
                printf("[SERVER] Max clients reached, dropping packet from %s:%d\n", inet_ntoa(client_ip), ntohs(client_port));
                continue;
            }
        }

		// Извлекаем данные из пакета
        int udp_data_len = ntohs(udp->len) - sizeof(struct udp_header);
        char *data = (char *)(buffer + iphdrlen + sizeof(struct udp_header));

        printf("[SERVER] Received from %s:%d (Клиент %d) — \"%.*s\"\n", inet_ntoa(client_ip), ntohs(client_port), client_idx + 1, udp_data_len, data);

		// Обрабатываем сообщение "exit"
        if (udp_data_len == 4 && strncmp(data, "exit", 4) == 0) {
            remove_client(client_idx);
            continue;
        }

        clients[client_idx].msg_count++;
        if (clients[client_idx].msg_count >= 10) {
            printf("[SERVER] Клиент %s:%d (Клиент %d) достиг лимита сообщений, удаляется.\n", inet_ntoa(client_ip), ntohs(client_port), client_idx + 1);
            remove_client(client_idx);
            continue;
        }

		// Формируем ответ
        char response[1024];
        int resp_len = snprintf(response, sizeof(response), "%.*s %d", udp_data_len, data, clients[client_idx].msg_count);

		// Формируем ответный пакет
        unsigned char sendbuf[1500];
        memset(sendbuf, 0, sizeof(sendbuf));

        struct ip_header *rip = (struct ip_header *)sendbuf;
        struct udp_header *rudp = (struct udp_header *)(sendbuf + sizeof(struct ip_header));
        char *rdata = (char *)(sendbuf + sizeof(struct ip_header) + sizeof(struct udp_header));
        memcpy(rdata, response, resp_len);

		// Заполняем IP-заголовок
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

		//UDP-заголовок
        rudp->source = htons(SERVER_PORT);
        rudp->dest = client_port;
        rudp->len = htons(sizeof(struct udp_header) + resp_len);
        rudp->check = 0;
        rudp->check = udp_checksum(rip, rudp, (unsigned char *)rdata, resp_len);

        struct sockaddr_in to_addr;
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = client_port;
        to_addr.sin_addr = client_ip;

		// Отправляем ответ
        ssize_t sent = sendto(sockfd, sendbuf, sizeof(struct ip_header) + sizeof(struct udp_header) + resp_len,
                              0, (struct sockaddr *)&to_addr, sizeof(to_addr));
        if (sent < 0)
            perror("sendto");
        else
            printf("[SERVER] Sent response to %s:%d (Клиент %d) — \"%.*s\"\n", inet_ntoa(client_ip), ntohs(client_port), client_idx + 1, resp_len, response);
    }

    close(sockfd); // Закрываем сокет
    return 0;
}
