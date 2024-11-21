#include "semaphore_two.h"

#define FILENAME "numbers.txt"
#define NUM_CHILDREN 2

void child_process(int semid, int child_num, int iterations) {
    for (int i = 0; i < iterations; i++) {
        lock_semaphore(semid, child_num); // Ждем разрешения от родителя

        // Читаем число из файла
        int file_fd = open(FILENAME, O_RDONLY);
        if (file_fd == -1) {
            error_exit("Ошибка открытия файла в дочернем процессе");
        }

        char buffer[128];
        lseek(file_fd, 0, SEEK_SET); // Сбрасываем указатель чтения на начало файла
        read(file_fd, buffer, sizeof(buffer));
        close(file_fd);

        // Удаляем лишние символы новой строки
        buffer[strcspn(buffer, "\n")] = '\0';

        int number = atoi(buffer);
        printf("Дочерний процесс %d прочитал число: %d\n", child_num, number);

        // Изменяем число
        number += child_num + 1; // +1 для первого процесса, +2 для второго
        printf("Дочерний процесс %d отправляет число: %d\n", child_num, number);

        // Разблокируем родителя
        unlock_semaphore(semid, NUM_CHILDREN);
    }
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

    key_t key = create_semaphore_key("/tmp", 'A');
    int semid = create_semaphore(key, NUM_CHILDREN + 1); // Один семафор для каждого дочернего процесса + один для родителя

    // Инициализируем семафоры
    for (int i = 0; i < NUM_CHILDREN; i++) {
        initialize_semaphore(semid, i, 0); // Семафоры для детей заблокированы
    }
    initialize_semaphore(semid, NUM_CHILDREN, 1); // Семафор для родителя разблокирован

    // Создаем дочерние процессы
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (fork() == 0) {
            child_process(semid, i, iterations);
        }
    }

    // Родительский процесс
    for (int i = 0; i < iterations; i++) {
        // Генерация случайного числа
        int number = rand() % (100-10+1)+10; // Число от 0 до 99
        printf("Родительский процесс генерирует число: %d\n", number);

        // Записываем число в файл
        int file_fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (file_fd == -1) {
            error_exit("Ошибка открытия файла в родительском процессе");
        }

        char buffer[128];
        sprintf(buffer, "%d", number);
        write(file_fd, buffer, strlen(buffer));
        fsync(file_fd); // Гарантируем запись на диск
        close(file_fd);

        // Чередуем разблокировку дочерних процессов
        for (int j = 0; j < NUM_CHILDREN; j++) {
            unlock_semaphore(semid, j); // Разблокируем одного дочернего
            lock_semaphore(semid, NUM_CHILDREN); // Ждем ответа от дочернего процесса
        }
    }

    // Удаляем семафоры и ждем завершения дочерних процессов
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL); // Ожидание завершения каждого дочернего процесса
    }
    remove_semaphore(semid);

    return 0;
}
