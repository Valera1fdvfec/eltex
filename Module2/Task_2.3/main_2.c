#include <stdio.h>
#include <locale.h>
#include <string.h>
#include "calculator_2.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");  // Устанавливаем локаль для поддержки UTF-8

    // Создаем массив операций и их функций
    Operation operations[] = {
        { "+", add },
        { "-", subtract },
        { "*", multiply },
        { "/", divide }
    };
    int num_operations = sizeof(operations) / sizeof(operations[0]);

    double result = 0;
    double current_number = 0;
    char operation[100];

    printf("Добро пожаловать в калькулятор!\n");
    printf("Вводите сначала  одно число, следом операцию, которую хотите совершить и второе число!\n");

    while (1) {

    printf("Введите первое число: ");
    if (scanf("%lf", &result) == 1) {
        printf("Число введено верно!\n");
        break;
    } else {
 	printf("Ошибка ввода числа. Пожалуйста, введите число заново.\n");
	while (getchar() != '\n');
    }
}
	while (1) {

        printf("Введите операцию (+, -, *, /) или '0' для завершения: ");
        if (scanf("%99s", operation) != 1) {
            printf("Ошибка ввода операции.\n");
            continue;
        }

        // Проверяем, если введено "0", то выходим из программы
        if (strcmp(operation, "0") == 0) {
            printf("Конечный результат: %lf\n", result);
            break;
        }

        
        int operation_found = 0;
        operation_func selected_func = NULL;
        for (int i = 0; i < num_operations; i++) {
            if (strcmp(operation, operations[i].name) == 0) {
                selected_func = operations[i].func;
                operation_found = 1;
                break;
            }
        }

        if (!operation_found) {
            printf("Неверная операция.\n");
            continue;
        }

        
        printf("Введите следующее число: ");
        if (scanf("%lf", &current_number) != 1) {
            printf("Ошибка ввода числа.\n");
            continue;
        }

        result = selected_func(2, result, current_number);

        // Выводим текущий результат
        printf("Текущий результат: %lf\n", result);
    }

    return 0;
}
