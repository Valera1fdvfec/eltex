#include "file_permissions.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Функция для преобразования числового представления прав доступа в буквенное
void displayPermissions(mode_t mode) {
    printf("Режим доступа: ");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}

// Функция для отображения прав доступа в двоичном формате
void displayBinaryPermissions(mode_t mode) {
    printf("Битовое представление (двоичное): ");
    for (int i = 8; i >= 0; i--) {
        printf("%d", (mode & (1 << i)) ? 1 : 0);
        if (i % 3 == 0 && i != 0) {
            printf(" "); // Разделяем группы битов для удобства чтения
        }
    }
    printf("\n");
}

// Функция для получения и отображения прав доступа к файлу
void getPermissions(char *filename) {
    struct stat fileStat;
    if (stat(filename, &fileStat) < 0) {
        perror("Ошибка при получении информации о файле");
        return;
    }
    printf("Права доступа к файлу %s:\n", filename);
    displayPermissions(fileStat.st_mode); // Буквенное представление
    printf("Числовое представление: %o\n", fileStat.st_mode & 0777); // Числовое
    displayBinaryPermissions(fileStat.st_mode & 0777); // Двоичное представление
}

// Функция для установки новых прав доступа
void setPermissions(char *filename, mode_t mode) {
    if (chmod(filename, mode) < 0) {
        perror("Ошибка при изменении прав доступа");
    } else {
        printf("Права доступа к файлу успешно изменены.\n");
        getPermissions(filename); // Выводим обновленные права
    }
}
