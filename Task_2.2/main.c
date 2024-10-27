#include <locale.h>
#include "calculator.h"

// Функция для проверки операции
static int check_operation(const char* operation) {
    if (strcmp(operation, "+") == 0 || strcmp(operation, "-") == 0 ||
        strcmp(operation, "*") == 0 || strcmp(operation, "/") == 0) {
        return 1;
    }
    return 0;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    char operation[100];
    double result = 0;
    int numArgs = 0;
    double args[10] = { 0 };

    while (1) {
        printf("Введите операцию (+, -, *, /) или напишите: 'выход' для выхода: ");

        if (scanf("%99s", operation) != 1) {
            printf("Ошибка ввода операции.\n");
            continue;
        }

        printf("Введено: %s\n", operation);

        if (strcmp(operation, "выход") == 0) {
            printf("Программа завершена.\n");
            break;
        }

        if (!check_operation(operation)) {
            printf("Неверная операция.\n");
            continue;
        }

        printf("Сколько чисел будете вводить? ");
        if (scanf("%d", &numArgs) != 1 || numArgs <= 0 || numArgs > 10) {
            printf("Ошибка ввода. Введите число от 1 до 10.\n");
            continue;
        }

        for (int i = 0; i < numArgs; i++) {
            printf("Введите число %d: ", i + 1);
            if (scanf("%lf", &args[i]) != 1) {
                printf("Ошибка ввода числа.\n");
                break;
            }
        }

        if (strcmp(operation, "+") == 0) {
            result = add(numArgs, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
        }
        else if (strcmp(operation, "-") == 0) {
            result = subtract(numArgs, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
        }
        else if (strcmp(operation, "*") == 0) {
            result = multiply(numArgs, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
        }
        else if (strcmp(operation, "/") == 0) {
            result = divide(numArgs, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
            if (result == -1) {
                continue;
            }
        }

        printf("Результат: %lf\n", result);
    }

    return 0;
}
