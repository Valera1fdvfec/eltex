#ifndef CALCULATE_AREAS_H
#define CALCULATE_AREAS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// Функция для расчета площадей квадратов
void calculate_areas(int start, int end, char *argv[], int is_parent);

#endif // CALCULATE_AREAS_H
