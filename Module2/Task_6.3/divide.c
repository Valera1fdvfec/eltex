#include "operations.h"
#include <stdarg.h>
#include <stdio.h>

double divide(int count, ...) {
    va_list args;
    va_start(args, count);
    double result = va_arg(args, double);
    for (int i = 1; i < count; i++) {
        double next = va_arg(args, double);
        if (next == 0) {
            printf("Oшибка: деление на ноль! Операция пропущена.\n");
            va_end(args);
            return result;
        }
        result /= next;
    }
    va_end(args);
    return result;
}
