#ifndef POSIX_SEMAPHORES_H
#define POSIX_SEMAPHORES_H

// Общие заголовочные файлы
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

// Работа с файлами и процессами
#include <sys/stat.h>
#include <sys/wait.h>

// POSIX-семафоры
#include <semaphore.h>
#include <errno.h>

// Функции для работы с POSIX-семафорами
sem_t *create_semaphore(const char *name, unsigned int value);
void destroy_semaphore(const char *name, sem_t *sem);
void wait_semaphore(sem_t *sem);
void post_semaphore(sem_t *sem);

#endif // INCLUDES_H
