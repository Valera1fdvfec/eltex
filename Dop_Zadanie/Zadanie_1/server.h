#ifndef RAW_SOCKET_H
#define RAW_SOCKET_H

#include <netinet/in.h>

// IP header
struct ip_header {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error "Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
};

// UDP header
struct udp_header {
    uint16_t source;
    uint16_t dest;
    uint16_t len;
    uint16_t check;
};

#define SERVER_PORT 12345
#define CLIENT_PORT 54321
#define MAX_BUFFER_SIZE 4096

// Функция подсчёта checksum (IP, UDP)
unsigned short checksum(unsigned short *buf, int nwords);

// UDP checksum с псевдо-заголовком
unsigned short udp_checksum(struct ip_header *ip, struct udp_header *udp, unsigned char *payload, int payload_len);

#endif
