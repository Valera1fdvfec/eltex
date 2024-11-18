#include "semaphore.h"

// Операции над семафором
struct sembuf sem_lock = {0, -1, 0};  // Блокировка
struct sembuf sem_unlock = {0, 1, 0}; // Разблокировка

// Функция для обработки ошибок
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Создание ключа для семафора
key_t create_semaphore_key(const char *path, int id) {
    key_t key;
    if ((key = ftok(path, id)) == -1) {
        error_exit("Ошибка ftok");
    }
    return key;
}

// Создание семафора
int create_semaphore(key_t key) {
    int semid;
    if ((semid = semget(key, 1, IPC_CREAT | 0666)) == -1) {
        error_exit("Ошибка semget");
    }
    return semid;
}

// Инициализация семафора
void initialize_semaphore(int semid, int value) {
    if (semctl(semid, 0, SETVAL, value) == -1) {
        error_exit("Ошибка semctl");
    }
}

// Блокировка семафора
void lock_semaphore(int semid) {
    if (semop(semid, &sem_lock, 1) == -1) {
        error_exit("Ошибка semop (блокировка)");
    }
}

// Разблокировка семафора
void unlock_semaphore(int semid) {
    if (semop(semid, &sem_unlock, 1) == -1) {
        error_exit("Ошибка semop (разблокировка)");
    }
}

// Удаление семафора
void remove_semaphore(int semid) {
    if (semctl(semid, 0, IPC_RMID) == -1) {
        error_exit("Ошибка удаления семафора");
    }
}
