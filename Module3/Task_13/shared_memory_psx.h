#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#define SHM_NAME "/posix_shared_memory"
#define SEM_DATA_READY "/sem_data_ready"
#define SEM_RESULT_READY "/sem_result_ready"
#define SHM_SIZE sizeof(SharedMemory)

typedef struct {
    int data[100];
    int size;
    int min;
    int max;
} SharedMemory;

int create_shared_memory();
SharedMemory *attach_shared_memory(int fd);
void detach_shared_memory(SharedMemory *shm);
void destroy_shared_memory();
sem_t *create_semaphore(const char *name, int initial_value);
void close_semaphore(sem_t *sem, const char *name);

#endif
