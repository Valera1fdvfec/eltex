#include <sys/wait.h>
#include "my_shell.h"

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        // Вывод приглашения
        printf("my_shell> ");
        fflush(stdout);

        // Чтение строки ввода от пользователя
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Ошибка чтения строки");
            exit(1);
        }

        // Удаление символа новой строки
        input[strcspn(input, "\n")] = '\0';

        // Если введена команда "exit", завершаем работу
        if (strcmp(input, "exit") == 0) {
            printf("Выход из программы...\n");
            break;
        }

        // Парсинг ввода на команду и аргументы
        parse_input(input, args);

        // Создание дочернего процесса
        pid_t pid = fork();
        if (pid < 0) {
            perror("Ошибка создания процесса");
            exit(1);
        } else if (pid == 0) {
            // Дочерний процесс: выполняет команду
            if (execvp(args[0], args) < 0) {
                perror("Ошибка выполнения команды");
                exit(1);
            }
        } else {
            // Родительский процесс: ожидает завершения дочернего процесса
            wait(NULL);
        }
    }

    return 0;
}
