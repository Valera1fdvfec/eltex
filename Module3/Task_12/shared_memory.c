#include "shared_memory.h"

int create_shared_memory() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Ошибка при создании разделяемой памяти");
        exit(1);
    }
    return shmid;
}

void *attach_shared_memory(int shmid) {
    void *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1) {
        perror("Ошибка при подключении разделяемой памяти");
        exit(1);
    }
    return shmaddr;
}

void detach_shared_memory(void *shmaddr) {
    if (shmdt(shmaddr) < 0) {
        perror("Ошибка при отключении разделяемой памяти");
        exit(1);
    }
}

void destroy_shared_memory(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("Ошибка при удалении разделяемой памяти");
        exit(1);
    }
}
