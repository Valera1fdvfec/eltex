#include "signal.h"

// Глобальная переменная для синхронизации
volatile sig_atomic_t access_allowed = 0;
volatile sig_atomic_t child_done = 0;

void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("Дочерний процесс: получен SIGUSR1 (ожидание разрешения)\n");
        child_done = 1;  // Дочерний процесс завершил чтение и отправку числа
    } else if (sig == SIGUSR2) {
        printf("Дочерний процесс: получен SIGUSR2 (разрешение доступа)\n");
        access_allowed = 1;  // Дочерний процесс может читать файл
    }
}

void child_process(int pipe_fd, const char *file) {
    FILE *fp;

    for (;;) {
        // Ждем разрешения на доступ к файлу (SIGUSR2)
        printf("Дочерний процесс: ожидаем SIGUSR2...\n");
        while (!access_allowed) {
            pause(); // Ждем SIGUSR2
        }
        access_allowed = 0; // Сбрасываем флаг

        // Открываем файл для чтения
        fp = fopen(file, "r");
        if (fp == NULL) {
            perror("Ошибка открытия файла");
            continue;
        }

        // Считываем последнее число из файла
        int number, last_number = -1;
        while (fscanf(fp, "%d", &number) != EOF) {
            last_number = number;
        }
        fclose(fp);

        if (last_number == -1) {
            printf("Ошибка: файл пустой или не удалось считать число\n");
            continue;
        }

        printf("Дочерний процесс: считано число %d из файла\n", last_number);

        // Увеличиваем число и отправляем его родителю
        int new_number = last_number + 1;
        write(pipe_fd, &new_number, sizeof(new_number));
        printf("Дочерний процесс: отправлено новое число %d родителю\n", new_number);

        // Сообщаем родительскому процессу, что задача выполнена
        kill(getppid(), SIGUSR1);
    }
}

void parent_process(int pipe_fd, int count, pid_t child_pid) {
    FILE *file = fopen("numbers.txt", "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        // Генерируем случайное число
        int number = rand() % 100;
        printf("Родительский процесс: сгенерировано число %d\n", number);

        // Записываем число в файл
        fprintf(file, "%d\n", number);
        fflush(file);
        printf("Родительский процесс: число %d записано в файл\n", number);

        // Разрешаем дочернему процессу читать файл
        kill(child_pid, SIGUSR2);
        printf("Родительский процесс: отправлен SIGUSR2 (разрешение доступа)\n");

        // Ждем, пока дочерний процесс выполнит свою задачу
        while (!child_done) {
            pause(); // Ждем SIGUSR1 от дочернего процесса
        }
        child_done = 0; // Сбрасываем флаг после завершения работы дочернего процесса

        // Читаем новое число от дочернего процесса через канал
        int new_number;
        if (read(pipe_fd, &new_number, sizeof(new_number)) > 0) {
            printf("Родительский процесс: получено новое число %d от дочернего процесса\n", new_number);
        }

        printf("Родительский процесс: завершена итерация %d\n", i + 1);
    }

    // Уведомляем дочерний процесс о завершении работы
    int end_signal = -1;
    write(pipe_fd, &end_signal, sizeof(end_signal));
    fclose(file);
    printf("Родительский процесс завершил работу\n");
}
