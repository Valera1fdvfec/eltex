#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>

// Объявления функций
uint32_t ip_to_int(const char *ip_str);
uint32_t generate_random_ip();
int is_same_subnet(uint32_t ip, uint32_t gateway_ip, uint32_t subnet_mask);

#endif // NETWORK_H
