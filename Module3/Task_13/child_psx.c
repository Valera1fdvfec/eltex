#include "shared_memory_psx.h"

int main() {
    // Подключаемся к разделяемой памяти
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd < 0) {
        perror("Ошибка открытия разделяемой памяти");
        exit(1);
    }
    SharedMemory *shm = attach_shared_memory(fd);

    // Открываем семафоры
    sem_t *sem_data_ready = sem_open(SEM_DATA_READY, 0);
    if (sem_data_ready == SEM_FAILED) {
        perror("Ошибка подключения к семафору sem_data_ready");
        exit(1);
    }

    sem_t *sem_result_ready = sem_open(SEM_RESULT_READY, 0);
    if (sem_result_ready == SEM_FAILED) {
        perror("Ошибка подключения к семафору sem_result_ready");
        exit(1);
    }

    // Ждем сигнал от родителя
    sem_wait(sem_data_ready);

    // Обрабатываем данные
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

    // Уведомляем родителя
    sem_post(sem_result_ready);

    // Вывод содержимого разделяемой памяти
    printf("Содержимое разделяемой памяти:\n");
    printf("Размер: %d\n", shm->size);
    printf("Данные: ");
    for (int i = 0; i < shm->size; i++) {
        printf("%d ", shm->data[i]);
    }
    printf("\n");

    // Завершаем работу
    detach_shared_memory(shm);
    return 0;
}
