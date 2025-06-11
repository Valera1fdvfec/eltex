#include "server.h"
#include <string.h>

// Вычисление контрольной суммы для IP или UDP заголовков
unsigned short checksum(unsigned short *buf, int nwords) {
    unsigned long sum = 0; // Накопительная сумма
    for (int i = 0; i < nwords; i++)
        sum += ntohs(buf[i]); // Суммируем слова в сетевом порядке

    // Сворачиваем сумму до 16 бит
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return htons(~sum); // Инвертируем и возвращаем в сетевом порядке
}

// Вычисление контрольной суммы для UDP
unsigned short udp_checksum(struct ip_header *ip, struct udp_header *udp, unsigned char *payload, int payload_len) {
    // Псевдозаголовок для UDP контрольной суммы
    struct pseudo_header {
        uint32_t src_addr; // IP-адрес источника
        uint32_t dst_addr; // IP-адрес назначения
        uint8_t zero; // Зарезервированный ноль
        uint8_t protocol; // Протокол (UDP)
        uint16_t udp_len; // Длина UDP пакета
    } pseudo_header;

    // Заполняем псевдозаголовок
    pseudo_header.src_addr = ip->saddr;
    pseudo_header.dst_addr = ip->daddr;
    pseudo_header.zero = 0;
    pseudo_header.protocol = ip->protocol;
    pseudo_header.udp_len = htons(sizeof(struct udp_header) + payload_len);

    // Подготавливаем буфер для вычислений
    int total_len = sizeof(pseudo_header) + sizeof(struct udp_header) + payload_len;
    unsigned char buf[1500];
    memset(buf, 0, sizeof(buf)); // Очищаем буфер

    // Копируем псевдозаголовок, UDP-заголовок и данные
    memcpy(buf, &pseudo_header, sizeof(pseudo_header));
    memcpy(buf + sizeof(pseudo_header), udp, sizeof(struct udp_header));
    if (payload_len > 0)
        memcpy(buf + sizeof(pseudo_header) + sizeof(struct udp_header), payload, payload_len);

    // Вычисляем контрольную сумму
    unsigned short result = checksum((unsigned short *)buf, total_len / 2 + (total_len % 2));
    return result; // Возвращаем контрольную сумму
}
