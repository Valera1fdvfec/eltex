#include "shared_memory_psx.h"
#include <sys/wait.h>  // Для wait()

int main() {
    int fd = create_shared_memory();
    SharedMemory *shm = attach_shared_memory(fd);

    sem_t *sem_data_ready = create_semaphore(SEM_DATA_READY, 0);
    sem_t *sem_result_ready = create_semaphore(SEM_RESULT_READY, 0);

    srand(time(NULL));

    // Генерация данных
    shm->size = rand() % 10 + 5;  // Количество чисел (5-14)
    printf("Родительский процесс: Генерация %d чисел\n", shm->size);
    for (int i = 0; i < shm->size; i++) {
        shm->data[i] = rand() % 100;  // Генерация чисел от 0 до 99
        printf("%d ", shm->data[i]);
    }
    printf("\n");

    // Создаем дочерний процесс
    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка создания дочернего процесса");
        detach_shared_memory(shm);
        destroy_shared_memory();
        close_semaphore(sem_data_ready, SEM_DATA_READY);
        close_semaphore(sem_result_ready, SEM_RESULT_READY);
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс
        execl("./child_psx", "child_psx", NULL);
        perror("Ошибка выполнения дочернего процесса");
        exit(1);
    } else {
        // Родительский процесс
        sem_post(sem_data_ready);  // Сигнал дочернему процессу

        sem_wait(sem_result_ready);  // Ожидание результата
        printf("Родительский процесс: Минимум = %d, Максимум = %d\n", shm->min, shm->max);

        wait(NULL);  // Ждем завершения дочернего процесса

        // Завершаем работу
        detach_shared_memory(shm);
        close_semaphore(sem_data_ready, SEM_DATA_READY);
        close_semaphore(sem_result_ready, SEM_RESULT_READY);

        return 0;
    }
}
