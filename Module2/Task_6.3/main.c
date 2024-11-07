#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "operations.h"

int main() {
    char *operations[] = { "add", "subtract", "multiply", "divide" };
    int num_operations = sizeof(operations) / sizeof(operations[0]);

    void *handles[num_operations];
    operation_func funcs[num_operations];

    // Загружаем библиотеки
    for (int i = 0; i < num_operations; i++) {
        char lib_name[50];
        snprintf(lib_name, sizeof(lib_name), "./lib%s.so", operations[i]);
        handles[i] = dlopen(lib_name, RTLD_LAZY);
        if (!handles[i]) {
            fprintf(stderr, "Не удалось загрузить библиотеку %s: %s\n", lib_name, dlerror());
            exit(1);
        }

        funcs[i] = (operation_func)dlsym(handles[i], operations[i]);
        char *error = dlerror();
        if (error != NULL) {
            fprintf(stderr, "Не удалось загрузить функцию %s: %s\n", operations[i], error);
            exit(1);
        }
    }

    double result = 0;
    double current_number = 0;
    char operation[100];

    printf("Добро пожалoвать в калькулятор!\n");
    printf("Вводите сначала одно число, затем операцию, которую хотите совершить, и второе число!\n");

    // Получаем первое число
    printf("Введите первое число: ");
    scanf("%lf", &result);

    while (1) {
        printf("Введите операцию (+, -, *, /) или '0' для завершения: ");
        scanf("%99s", operation);

        if (strcmp(operation, "0") == 0) {
            printf("Конечный результат: %lf\n", result);
            break;
        }

        int operation_index = -1;
        if (strcmp(operation, "+") == 0) {
            operation_index = 0;
        } else if (strcmp(operation, "-") == 0) {
            operation_index = 1;
        } else if (strcmp(operation, "*") == 0) {
            operation_index = 2;
        } else if (strcmp(operation, "/") == 0) {
            operation_index = 3;
        } else {
            printf("Неверная операция.\n");
            continue;
        }

        printf("Введите следующее число: ");
        scanf("%lf", &current_number);

        result = funcs[operation_index](2, result, current_number);
        printf("Текущий результат: %lf\n", result);
    }

    // Закрываем библиотеки
    for (int i = 0; i < num_operations; i++) {
        dlclose(handles[i]);
    }

    return 0;
}
