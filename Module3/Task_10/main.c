#include "semaphore_psx.h"

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

    // Создаем два именованных POSIX семафора
    sem_t *sem_parent = sem_open("/sem_parent", O_CREAT | O_EXCL, 0666, 1);
    sem_t *sem_child = sem_open("/sem_child", O_CREAT | O_EXCL, 0666, 0);
    
    if (sem_parent == SEM_FAILED || sem_child == SEM_FAILED) {
        perror("Ошибка создания семафоров");
        sem_unlink("/sem_parent");
        sem_unlink("/sem_child");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        error_exit("Ошибка fork");
    }

    if (pid == 0) {
        // Дочерний процесс
        for (int i = 0; i < iterations; i++) {
            // Ожидание разблокировки от родителя
            sem_wait(sem_child);
            printf("[DEBUG] Дочерний процесс начал итерацию %d\n", i + 1);

            // Читаем последнее число из файла
            int file_fd = open(FILENAME, O_RDONLY);
            if (file_fd == -1) {
                error_exit("[ERROR] Дочерний процесс: ошибка открытия файла");
            }

            char buffer[128];
            int bytes_read = read(file_fd, buffer, sizeof(buffer) - 1);
            close(file_fd);

            if (bytes_read <= 0) {
                fprintf(stderr, "[ERROR] Дочерний процесс: ошибка чтения файла\n");
                exit(EXIT_FAILURE);
            }

            buffer[bytes_read] = '\0';
            int number = atoi(buffer);
            printf("Дочерний процесс прочитал число: %d\n", number);

            // Увеличиваем число на 1 и выводим его
            number += 1;
            printf("Дочерний процесс отправляет число: %d\n", number);

            // Разблокировка родителя
            printf("[DEBUG] Дочерний процесс завершил итерацию %d и разблокировал родителя\n", i + 1);
            sem_post(sem_parent);
        }

        // Закрываем семафоры для дочернего процесса
        sem_close(sem_parent);
        sem_close(sem_child);
        printf("[DEBUG] Дочерний процесс завершён.\n");
        exit(EXIT_SUCCESS);

    } else {
        // Родительский процесс
        srand(time(NULL)); // Инициализация генератора случайных чисел

        for (int i = 0; i < iterations; i++) {
            // Родительский процесс начинает выполнение итерации
            sem_wait(sem_parent);
            printf("[DEBUG] Родительский процесс начал итерацию %d\n", i + 1);

            // Генерация случайного числа
            int number = rand() % 100; // Число от 0 до 99
            printf("Родительский процесс генерирует число и записывает его в файл: %d\n", number);

            // Записываем число в файл
            int file_fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (file_fd == -1) {
                error_exit("[ERROR] Родительский процесс: ошибка открытия файла");
            }

            char buffer[128];
            sprintf(buffer, "%d", number);
            write(file_fd, buffer, strlen(buffer));
            close(file_fd);

            // Разблокировка дочернего процесса
            printf("[DEBUG] Родительский процесс завершил итерацию %d и разблокировал дочерний процесс\n", i + 1);
            sem_post(sem_child);
        }

        // Ждём завершения дочернего процесса
        wait(NULL);

        // Закрываем и удаляем семафоры
        sem_close(sem_parent);
        sem_close(sem_child);
        sem_unlink("/sem_parent");
        sem_unlink("/sem_child");
        printf("[DEBUG] Родительский процесс завершён.\n");
    }

    return 0;
}
