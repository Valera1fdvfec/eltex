#include "kanal.h"

void generate_numbers(int write_fd, int count) {
    srand(getpid()); // Инициализация генератора случайных чисел
    for (int i = 0; i < count; i++) {
        int number = rand() % 100; // Генерация случайного числа
        write(write_fd, &number, sizeof(number));
    }
    close(write_fd); // Закрытие дескриптора записи
}

void parent_process(int read_fd, int count) {
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        int number;
        if (read(read_fd, &number, sizeof(number)) > 0) {
            printf("Число: %d\n", number); // Вывод на экран
            fprintf(file, "Число: %d\n", number); // Запись в файл
        }
    }

    close(read_fd); // Закрытие дескриптора чтения
    fclose(file);   // Закрытие файла
}
