#include "kanal.h"

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

    int pipe_fds[2];
    if (pipe(pipe_fds) == -1) {
        perror("Ошибка создания канала");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Дочерний процесс
        close(pipe_fds[0]); // Закрываем чтение
        generate_numbers(pipe_fds[1], count);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс
        close(pipe_fds[1]); // Закрываем запись
        parent_process(pipe_fds[0], count);
    }

    return 0;
}
