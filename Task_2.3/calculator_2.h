#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdarg.h>

typedef double (*operation_func)(int, ...);

typedef struct {
    const char *name;
    operation_func func;
} Operation;

double add(int count, ...);
double subtract(int count, ...);
double multiply(int count, ...);
double divide(int count, ...);

#endif // OPERATIONS_H
