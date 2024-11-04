#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

// Функция для преобразования строки IP-адреса в 32-битное число
uint32_t ip_to_int(const char *ip_str) {
    struct in_addr ip;
    if (inet_pton(AF_INET, ip_str, &ip) == 0) {
        fprintf(stderr, "Неверный IP-адрес: %s\n", ip_str);
        exit(EXIT_FAILURE);
    }
    return ntohl(ip.s_addr);
}

// Функция для генерации случайного IP-адреса
uint32_t generate_random_ip() {
    return (rand() % 256) << 24 | (rand() % 256) << 16 | (rand() % 256) << 8 | (rand() % 256);
}

// Функция для проверки, находится ли IP-адрес в одной подсети с маршрутизатором
int is_same_subnet(uint32_t ip, uint32_t gateway_ip, uint32_t subnet_mask) {
    return (ip & subnet_mask) == (gateway_ip & subnet_mask);
}
