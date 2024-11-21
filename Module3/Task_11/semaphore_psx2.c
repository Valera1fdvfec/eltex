#include "semaphore_psx2.h"

// Создание POSIX-семафора
sem_t *create_semaphore(const char *name, unsigned int value) {
    // Удаляем существующий семафор, если он есть
    sem_unlink(name);

    // Создаем новый семафор
    sem_t *sem = sem_open(name, O_CREAT | O_EXCL, 0666, value);
    if (sem == SEM_FAILED) {
        perror("Ошибка создания семафора");
        exit(EXIT_FAILURE);
    }
    return sem;
}

// Удаление POSIX-семафора
void destroy_semaphore(const char *name, sem_t *sem) {
    if (sem_close(sem) == -1) {
        perror("Ошибка закрытия семафора");
    }
    if (sem_unlink(name) == -1) {
        perror("Ошибка удаления семафора");
    }
}

// Ожидание семафора
void wait_semaphore(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        perror("Ошибка ожидания семафора");
        exit(EXIT_FAILURE);
    }
}

// Разблокировка семафора
void post_semaphore(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Ошибка разблокировки семафора");
        exit(EXIT_FAILURE);
    }
}
