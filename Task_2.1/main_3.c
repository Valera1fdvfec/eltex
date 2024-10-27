#include <stdio.h>
#include "contact_3.h"

int main() {
    initialize_locale();
    load_contacts();

    int choice;
    do {
        printf("\nМеню телефонной книги:\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать контакты\n");
        printf("3. Редактировать контакт\n");
        printf("4. Удалить контакт\n");
        printf("5. Выйти\n");
        printf("Выберите пункт меню: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                add_contact();
                break;
            case 2:
                display_contacts();
                break;
            case 3:
                edit_contact();
                break;
            case 4:
                delete_contact();
                break;
            case 5:
                printf("Завершение программы.\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 5);

    return 0;
}

