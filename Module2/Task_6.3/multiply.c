#include "operations.h"
#include <stdarg.h>

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
