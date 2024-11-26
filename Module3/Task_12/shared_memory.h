#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define SHM_KEY 12345
#define SHM_SIZE 1024

typedef struct {
    int data[100];
    int size;
    int min;
    int max;
} SharedMemory;

int create_shared_memory();
void *attach_shared_memory(int shmid);
void detach_shared_memory(void *shmaddr);
void destroy_shared_memory(int shmid);

#endif
