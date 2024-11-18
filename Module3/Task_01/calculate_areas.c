#include <stdio.h>
#include <stdlib.h>
#include "calculate_areas.h"

void calculate_areas(int start, int end, char *argv[], int is_parent) {
    if (is_parent) {
        printf("Родительский процесс (PID: %d) вычисляет площади квадратов:\n", getpid());
    } else {
        printf("Дочерний процесс (PID: %d) вычисляет площади квадратов:\n", getpid());
    }

    for (int i = start; i < end; i++) {
        int side = atoi(argv[i + 1]);
        int area = side * side;
        printf("Квадрат %d: длина стороны = %d, площадь = %d\n", i + 1, side, area);
    }
}
