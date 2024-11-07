#include "operations.h"
#include <stdarg.h>

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
