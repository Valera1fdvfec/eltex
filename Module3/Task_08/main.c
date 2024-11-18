#include "semaphore.h"

#define FILENAME "numbers.txt"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <число итераций>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int iterations = atoi(argv[1]);
    if (iterations <= 0) {
        fprintf(stderr, "Число итераций должно быть положительным.\n");
        exit(EXIT_FAILURE);
    }

    key_t key = create_semaphore_key("/tmp", 'A');
    int semid = create_semaphore(key);
    initialize_semaphore(semid, 0); // Семафор изначально заблокирован

    pid_t pid = fork();
    if (pid == -1) {
        error_exit("Ошибка fork");
    }

    if (pid == 0) {
        // Дочерний процесс
        for (int i = 0; i < iterations; i++) {
            lock_semaphore(semid); // Ждем, пока родитель разблокирует семафор

            // Читаем последнее число из файла
            int file_fd = open(FILENAME, O_RDONLY);
            if (file_fd == -1) {
                error_exit("Ошибка открытия файла в дочернем процессе");
            }

            char buffer[128];
            read(file_fd, buffer, sizeof(buffer));
            close(file_fd);

            int number = atoi(buffer);
            printf("Дочерний процесс прочитал число: %d\n", number);

            // Увеличиваем число на 1 и передаем родителю
            number += 1;
            printf("Дочерний процесс отправляет число: %d\n", number);

            unlock_semaphore(semid); // Разблокируем семафор для родителя
        }
        exit(EXIT_SUCCESS);

    } else {
        // Родительский процесс
        srand(time(NULL)); // Инициализация генератора случайных чисел

        for (int i = 0; i < iterations; i++) {
            // Генерация случайного числа
            int number = rand() % 100; // Число от 0 до 99
            printf("Родительский процесс генерирует число: %d\n", number);

            // Записываем число в файл
            int file_fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (file_fd == -1) {
                error_exit("Ошибка открытия файла в родительском процессе");
            }

            char buffer[128];
            sprintf(buffer, "%d", number);
            write(file_fd, buffer, strlen(buffer));
            close(file_fd);

            // Разблокируем семафор для дочернего процесса
            unlock_semaphore(semid);

            // Ждем завершения работы дочернего процесса
            lock_semaphore(semid);
        }

        // Удаляем семафор
        remove_semaphore(semid);
    }

    return 0;
}
