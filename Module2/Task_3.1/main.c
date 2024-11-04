#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "file_permissions.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: %s <имя_файла> <права (в формате rwxrwxrwx или числовом)>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    char *permissions = argv[2];
    mode_t mode = 0;

    // Проверка формата прав доступа (буквенное или числовое представление)
    if (strlen(permissions) == 9) {
        // Преобразование буквенного представления в числовое
        mode |= (permissions[0] == 'r') ? S_IRUSR : 0;
        mode |= (permissions[1] == 'w') ? S_IWUSR : 0;
        mode |= (permissions[2] == 'x') ? S_IXUSR : 0;
        mode |= (permissions[3] == 'r') ? S_IRGRP : 0;
        mode |= (permissions[4] == 'w') ? S_IWGRP : 0;
        mode |= (permissions[5] == 'x') ? S_IXGRP : 0;
        mode |= (permissions[6] == 'r') ? S_IROTH : 0;
        mode |= (permissions[7] == 'w') ? S_IWOTH : 0;
        mode |= (permissions[8] == 'x') ? S_IXOTH : 0;
    } else {
        // Преобразование числового представления в mode_t
        mode = strtol(permissions, NULL, 8);
        if (errno == EINVAL || errno == ERANGE) {
            perror("Неверный формат прав доступа");
            exit(EXIT_FAILURE);
        }
    }

    // Вывод начальных прав доступа
    printf("Начальные права доступа:\n");
    getPermissions(filename);

    // Изменение прав доступа
    setPermissions(filename, mode);

    return 0;
}
