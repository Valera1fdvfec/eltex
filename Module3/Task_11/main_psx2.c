#include "semaphore_psx2.h"

#define FILENAME "numbers.txt"
#define NUM_CHILDREN 2
#define SEM_PARENT "/sem_parent"
#define SEM_CHILD_0 "/sem_child_0"
#define SEM_CHILD_1 "/sem_child_1"

// Функция для дочернего процесса
void child_process(const char *sem_parent_name, const char *sem_child_name, int child_num, int iterations) {
    sem_t *sem_parent = sem_open(sem_parent_name, 0);
    sem_t *sem_child = sem_open(sem_child_name, 0);

    if (sem_parent == SEM_FAILED || sem_child == SEM_FAILED) {
        perror("Ошибка открытия семафора в дочернем процессе");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < iterations; i++) {
        wait_semaphore(sem_child); // Ждем разрешения от родителя

        // Читаем число из файла
        int file_fd = open(FILENAME, O_RDONLY);
        if (file_fd == -1) {
            perror("Ошибка открытия файла в дочернем процессе");
            exit(EXIT_FAILURE);
        }

        char buffer[128];
        read(file_fd, buffer, sizeof(buffer));
        close(file_fd);

        // Удаляем символ новой строки
        buffer[strcspn(buffer, "\n")] = '\0';

        int number = atoi(buffer);
        printf("Дочерний процесс %d прочитал число: %d\n", child_num, number);

        // Изменяем число
        number += child_num + 1; // +1 для первого процесса, +2 для второго
        printf("Дочерний процесс %d отправляет число: %d\n", child_num, number);

        post_semaphore(sem_parent); // Уведомляем родителя
    }

    sem_close(sem_child);
    sem_close(sem_parent);
    exit(EXIT_SUCCESS);
}

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

    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Удаляем старые семафоры, если они есть
    sem_unlink(SEM_PARENT);
    sem_unlink(SEM_CHILD_0);
    sem_unlink(SEM_CHILD_1);

    // Создаем POSIX-семафоры
    sem_t *sem_parent = create_semaphore(SEM_PARENT, 1); // Родительский семафор
    sem_t *sem_child_0 = create_semaphore(SEM_CHILD_0, 0); // Семафор для первого дочернего процесса
    sem_t *sem_child_1 = create_semaphore(SEM_CHILD_1, 0); // Семафор для второго дочернего процесса

    // Создаем дочерние процессы
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (fork() == 0) {
            if (i == 0) {
                child_process(SEM_PARENT, SEM_CHILD_0, i, iterations);
            } else {
                child_process(SEM_PARENT, SEM_CHILD_1, i, iterations);
            }
        }
    }

    // Родительский процесс
    for (int i = 0; i < iterations; i++) {
        // Генерация случайного числа от 10 до 100
        int number = rand() % (100 - 10 + 1) + 10;
        printf("Родительский процесс генерирует число: %d\n", number);

        // Записываем число в файл
        int file_fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (file_fd == -1) {
            perror("Ошибка открытия файла в родительском процессе");
            exit(EXIT_FAILURE);
        }

        char buffer[128];
        sprintf(buffer, "%d", number);
        write(file_fd, buffer, strlen(buffer));
        fsync(file_fd); // Гарантируем запись на диск
        close(file_fd);

        // Синхронизируем с дочерними процессами
        post_semaphore(sem_child_0); // Разрешаем первому процессу
        wait_semaphore(sem_parent); // Ждем ответа от первого процесса

        post_semaphore(sem_child_1); // Разрешаем второму процессу
        wait_semaphore(sem_parent); // Ждем ответа от второго процесса
    }

    // Ждем завершения дочерних процессов
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    // Удаляем семафоры
    destroy_semaphore(SEM_PARENT, sem_parent);
    destroy_semaphore(SEM_CHILD_0, sem_child_0);
    destroy_semaphore(SEM_CHILD_1, sem_child_1);

    return 0;
}
