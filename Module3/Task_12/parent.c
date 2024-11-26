#include "shared_memory.h"

int main() {
    int shmid = create_shared_memory();
    SharedMemory *shm = (SharedMemory *)attach_shared_memory(shmid);

    srand(time(NULL));
    shm->size = rand() % 10 + 5;  // Генерация количества чисел (от 5 до 14)
    printf("Родительский процесс: Генерация %d чисел\n", shm->size);

    for (int i = 0; i < shm->size; i++) {
        shm->data[i] = rand() % 100;  // Генерация чисел от 0 до 99
        printf("%d ", shm->data[i]);
    }
    printf("\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка при создании дочернего процесса");
        detach_shared_memory(shm);
        destroy_shared_memory(shmid);
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс
        execl("./child", "child", NULL);
        perror("Ошибка при выполнении дочернего процесса");
        exit(1);
    } else {
        // Родительский процесс ждет завершения дочернего
        wait(NULL);
        printf("Родительский процесс: Минимум = %d, Максимум = %d\n", shm->min, shm->max);

        detach_shared_memory(shm);
        destroy_shared_memory(shmid);
    }

    return 0;
}
