#include "kanal.h"
#include <errno.h>

// Генерация случайных чисел и запись в канал
void generate_numbers(int write_fd, int count) {
    srand(getpid()); // Инициализация генератора случайных чисел
    for (int i = 0; i < count; i++) {
        int number = rand() % 100; // Генерация случайного числа
        if (write(write_fd, &number, sizeof(number)) != sizeof(number)) {
            perror("Ошибка записи в канал");
            close(write_fd); // Попытка закрыть дескриптор даже при ошибке
            exit(EXIT_FAILURE);
        }
    }
    if (close(write_fd) == -1) { // Проверка закрытия дескриптора
        perror("Ошибка закрытия дескриптора записи");
        exit(EXIT_FAILURE);
    }
}

// Чтение из канала и запись чисел в файл
void parent_process(int read_fd, int count) {
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) { // Проверка открытия файла
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        int number;
        if (read(read_fd, &number, sizeof(number)) > 0) {
            printf("Родитель вывел и записал в файл число переданное дочерним процессом: %d\n", number); // Вывод на экран
            if (fprintf(file, "Число: %d\n", number) < 0) { // Проверка записи в файл
                perror("Ошибка записи в файл");
                fclose(file);   // Попытка закрыть файл даже при ошибке
                close(read_fd); // Попытка закрыть дескриптор чтения
                exit(EXIT_FAILURE);
            }
        } else if (errno != 0) { // Если read вернул ошибку
            perror("Ошибка чтения из канала");
            fclose(file);
            close(read_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (close(read_fd) == -1) { // Проверка закрытия дескриптора чтения
        perror("Ошибка закрытия дескриптора чтения");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (fclose(file) == EOF) { // Проверка закрытия файла
        perror("Ошибка закрытия файла");
        exit(EXIT_FAILURE);
    }
}
