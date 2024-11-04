#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "network.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Использование: %s <IP_шлюза> <Маска_подсети> <Количество_пакетов>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Считывание и преобразование аргументов командной строки
    uint32_t gateway_ip = ip_to_int(argv[1]);
    uint32_t subnet_mask = ip_to_int(argv[2]);
    int num_packets = atoi(argv[3]);
    if (num_packets <= 0) {
        fprintf(stderr, "Количество пакетов должно быть положительным числом\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL)); // Инициализация генератора случайных чисел

    int same_subnet_count = 0;
    int different_subnet_count = 0;

    // Генерация и обработка пакетов
    for (int i = 0; i < num_packets; i++) {
        uint32_t dest_ip = generate_random_ip();

        // Проверка принадлежности к подсети
        if (is_same_subnet(dest_ip, gateway_ip, subnet_mask)) {
            same_subnet_count++;
        } else {
            different_subnet_count++;
        }
    }

    // Вывод статистики
    printf("Количество пакетов для узлов \"своей\" подсети: %d (%.2f%%)\n",
           same_subnet_count, (same_subnet_count * 100.0) / num_packets);
    printf("Количество пакетов для узлов других сетей: %d (%.2f%%)\n",
           different_subnet_count, (different_subnet_count * 100.0) / num_packets);

    return EXIT_SUCCESS;
}
