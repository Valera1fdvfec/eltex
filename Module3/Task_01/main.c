#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "calculate_areas.h"

int main(int argc, char *argv[]) {
    // Проверка количества аргументов
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <длина стороны 1> <длина стороны 2> ...\n", argv[0]);
        return 1;
    }

    int num_squares = argc - 1; // Количество квадратов
    int mid = num_squares / 2;  // Середина для разделения задач
    pid_t pid = fork();         // Создание дочернего процесса

    if (pid < 0) {
        // Ошибка при создании процесса
        perror("Ошибка создания процесса");
        return 1;
    } else if (pid == 0) {
        // Дочерний процесс
        calculate_areas(mid, num_squares, argv, 0);
        exit(0); // Завершение дочернего процесса
    } else {
        // Родительский процесс
        calculate_areas(0, mid, argv, 1);
        // Ожидание завершения дочернего процесса
        wait(NULL);
    }

    return 0;
}
