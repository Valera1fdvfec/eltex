#include <stdio.h>
#include <string.h>
#include "my_shell.h"

void parse_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " \n");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        args[++i] = strtok(NULL, " \n");
    }
    args[i] = NULL; // Последний элемент должен быть NULL для execvp
}
