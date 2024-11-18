#ifndef MY_SHELL_H
#define MY_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_ARGS 10

// Функция для парсинга введённой строки на команду и аргументы
void parse_input(char *input, char **args);

#endif // MY_SHELL_H
