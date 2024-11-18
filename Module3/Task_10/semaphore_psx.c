#include "semaphore_psx.h"

// Функция для обработки ошибок
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Инициализация POSIX семафора
void initialize_semaphore(sem_t *sem, int value) {
    if (sem_init(sem, 1, value) == -1) { // 1 означает, что семафор будет разделяемым между процессами
        error_exit("Ошибка инициализации POSIX семафора");
    }
}

// Блокировка POSIX семафора
void lock_semaphore(sem_t *sem) {
    if (sem_wait(sem) == -1) { // Ожидание разблокировки
        error_exit("Ошибка блокировки POSIX семафора");
    }
}

// Разблокировка POSIX семафора
void unlock_semaphore(sem_t *sem) {
    if (sem_post(sem) == -1) { // Разблокировка семафора
        error_exit("Ошибка разблокировки POSIX семафора");
    }
}

// Уничтожение POSIX семафора
void destroy_semaphore(sem_t *sem) {
    if (sem_destroy(sem) == -1) {
        error_exit("Ошибка уничтожения POSIX семафора");
    }
}
