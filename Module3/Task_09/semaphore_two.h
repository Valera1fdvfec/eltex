#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

// Структура для операций над семафором
extern struct sembuf sem_lock;
extern struct sembuf sem_unlock;

// Функции управления семафорами
void error_exit(const char *msg);
key_t create_semaphore_key(const char *path, int id);
int create_semaphore(key_t key, int nsems);
void initialize_semaphore(int semid, int semnum, int value);
void lock_semaphore(int semid, int semnum);
void unlock_semaphore(int semid, int semnum);
void remove_semaphore(int semid);

#endif // SEMAPHORE_H
