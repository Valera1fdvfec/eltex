#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define BUF_SIZE 65536
#define SERVER_PORT 2010

void process_packet(unsigned char *buffer, int size) {
    struct iphdr *ip_header = (struct iphdr *)buffer; // Заголовок IP
    struct udphdr *udp_header = (struct udphdr *)(buffer + ip_header->ihl * 4); // Заголовок UDP

    // Только пакеты, предназначенные для указанного порта
    if (ntohs(udp_header->dest) == SERVER_PORT) {
        printf("Захвачен пакет для порта %d:\n", SERVER_PORT);

        // Вывод IP-адресов отправителя и получателя
        struct in_addr src_addr, dest_addr;
        src_addr.s_addr = ip_header->saddr;
        dest_addr.s_addr = ip_header->daddr;
        printf("Отправитель: %s\n", inet_ntoa(src_addr));
        printf("Получатель: %s\n", inet_ntoa(dest_addr));

        // Вывод полезной нагрузки
        unsigned char *data = buffer + ip_header->ihl * 4 + sizeof(struct udphdr);
        int data_size = ntohs(udp_header->len) - sizeof(struct udphdr);

        printf("Данные (%d байт): ", data_size);
        for (int i = 0; i < data_size; i++) {
            printf("%c", data[i]);
        }
        printf("\n\n");

        // Сохранение данных в файл
        FILE *f = fopen("dump.txt", "a");
        if (f != NULL) {
            fprintf(f, "Отправитель: %s\n", inet_ntoa(src_addr));
            fprintf(f, "Получатель: %s\n", inet_ntoa(dest_addr));
            fprintf(f, "Данные: ");
            for (int i = 0; i < data_size; i++) {
                fprintf(f, "%c", data[i]);
            }
            fprintf(f, "\n\n");
            fclose(f);
        }
    }
}

int main() {
    int raw_sock;
    unsigned char *buffer = (unsigned char *)malloc(BUF_SIZE);
    struct sockaddr_in saddr;
    socklen_t saddr_size = sizeof(saddr);

    // Создание RAW сокета
    raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock < 0) {
        perror("Ошибка при создании RAW сокета");
        return 1;
    }

    printf("RAW сокет создан. Ожидание пакетов...\n");

    while (1) {
        // Захват пакета
        int data_size = recvfrom(raw_sock, buffer, BUF_SIZE, 0, (struct sockaddr *)&saddr, &saddr_size);
        if (data_size < 0) {
            perror("Ошибка при получении пакета");
            break;
        }

        // Обработка пакета
        process_packet(buffer, data_size);
    }

    close(raw_sock);
    free(buffer);
    return 0;
}
