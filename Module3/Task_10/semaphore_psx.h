#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

// Функции управления POSIX семафорами
void error_exit(const char *msg);
void initialize_semaphore(sem_t *sem, int value);
void lock_semaphore(sem_t *sem);
void unlock_semaphore(sem_t *sem);
void destroy_semaphore(sem_t *sem);

#endif
