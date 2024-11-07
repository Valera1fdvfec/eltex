#ifndef OPERATIONS_H
#define OPERATIONS_H

// Определение типа функции
typedef double (*operation_func)(int, ...);

// Прототипы функций операций
double add(int count, ...);
double subtract(int count, ...);
double multiply(int count, ...);
double divide(int count, ...);

#endif // OPERATIONS_H
