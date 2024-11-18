#include "signal.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <количество чисел>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int count = atoi(argv[1]);
    if (count <= 0) {
        fprintf(stderr, "Количество чисел должно быть положительным!\n");
        exit(EXIT_FAILURE);
    }

    // Создаем канал
    int pipe_fds[2];
    if (pipe(pipe_fds) == -1) {
        perror("Ошибка создания канала");
        exit(EXIT_FAILURE);
    }

    // Создаем дочерний процесс
    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Дочерний процесс
        close(pipe_fds[1]); // Закрываем запись в канале
        signal(SIGUSR1, handle_signal); // Устанавливаем обработчик SIGUSR1
        signal(SIGUSR2, handle_signal); // Устанавливаем обработчик SIGUSR2
        child_process(pipe_fds[0], "numbers.txt");
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс
        close(pipe_fds[0]); // Закрываем чтение из канала
        srand(getpid());    // Инициализируем генератор случайных чисел
        signal(SIGUSR1, handle_signal); // Родитель также устанавливает обработчик для получения уведомлений от дочернего процесса
        parent_process(pipe_fds[1], count, pid);
    }

    return 0;
}
