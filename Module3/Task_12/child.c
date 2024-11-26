#include "shared_memory.h"

int main() {
    int shmid = create_shared_memory();
    SharedMemory *shm = (SharedMemory *)attach_shared_memory(shmid);

    printf("Дочерний процесс: Обработка данных\n");
    shm->min = shm->data[0];
    shm->max = shm->data[0];

    for (int i = 1; i < shm->size; i++) {
        if (shm->data[i] < shm->min) {
            shm->min = shm->data[i];
        }
        if (shm->data[i] > shm->max) {
            shm->max = shm->data[i];
        }
    }

    printf("Дочерний процесс: Минимум = %d, Максимум = %d\n", shm->min, shm->max);

    detach_shared_memory(shm);
    return 0;
}
