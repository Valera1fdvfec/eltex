#include "shared_memory_psx.h"

int create_shared_memory() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("Ошибка создания разделяемой памяти");
        exit(1);
    }
    ftruncate(fd, SHM_SIZE);
    return fd;
}

SharedMemory *attach_shared_memory(int fd) {
    void *addr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("Ошибка подключения разделяемой памяти");
        exit(1);
    }
    return (SharedMemory *)addr;
}

void detach_shared_memory(SharedMemory *shm) {
    munmap(shm, SHM_SIZE);
}

void destroy_shared_memory() {
    shm_unlink(SHM_NAME);
}

sem_t *create_semaphore(const char *name, int initial_value) {
    sem_t *sem = sem_open(name, O_CREAT, 0666, initial_value);
    if (sem == SEM_FAILED) {
        perror("Ошибка создания семафора");
        exit(1);
    }
    return sem;
}

void close_semaphore(sem_t *sem, const char *name) {
    sem_close(sem);
    sem_unlink(name);
}
