#include "calculator.h"

double add(int count, ...) {
    va_list args;
    va_start(args, count);
    double sum = 0;

    for (int i = 0; i < count; i++) {
        sum += va_arg(args, double);
    }

    va_end(args);
    return sum;
}

double subtract(int count, ...) {
    va_list args;
    va_start(args, count);
    double result = va_arg(args, double);

    for (int i = 1; i < count; i++) {
        result -= va_arg(args, double);
    }

    va_end(args);
    return result;
}

double multiply(int count, ...) {
    va_list args;
    va_start(args, count);
    double result = 1;

    for (int i = 0; i < count; i++) {
        result *= va_arg(args, double);
    }

    va_end(args);
    return result;
}

double divide(int count, ...) {
    va_list args;
    va_start(args, count);
    double result = va_arg(args, double);

    for (int i = 1; i < count; i++) {
        double next = va_arg(args, double);
        if (next == 0) {
            printf("Ошибка: деление на ноль!\n");
            va_end(args);
            return -1;
        }
        result /= next;
    }

    va_end(args);
    return result;
}
