#include "contact.h"
#include <locale.h>

#define FILENAME "contacts.txt"
#define NEW_FILENAME "updated_contacts.txt"

void print_contacts(Contact *contacts, int count) {
    for (int i = 0; i < count; i++) {
        Contact *contact = &contacts[i];  // Получаем текущий контакт

        printf("\nКонтакт %d\n", i + 1);
        printf("Фамилия: %s\n", contact->surname);
        printf("Имя: %s\n", contact->name);
        printf("Отчество: %s\n", contact->middle_name);
        printf("Компания: %s\n", contact->company);
        printf("Должность: %s\n", contact->job_position);
        printf("Номер телефона: %s\n", contact->phone_number);
        printf("Электронная почта: %s\n", contact->email);
        printf("Ссылка на соцсети: %s\n", contact->social_link);
        printf("Профиль в мессенджере: %s\n", contact->messenger_profile);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    Contact contacts[MAX_CONTACTS];
    int count = 0;

    load_contacts(FILENAME, contacts, &count);

    int choice, index;
    Contact temp_contact;

    while (1) {
        printf("\nДобро пожаловать в менеджер контактов\n");
        printf("1. Добавить контакт\n");
        printf("2. Редактировать контакт\n");
        printf("3. Удалить контакт\n");
        printf("4. Просмотреть контакты\n");
        printf("5. Выход\n");
        printf("Выберите опцию: ");

        if (scanf("%d", &choice) != 1) { // Проверяем успешность ввода
            printf("Некорректный выбор. Попробуйте снова.\n");
            while (getchar() != '\n'); // Очищаем буфер ввода
            continue;
        }

        switch (choice) {
            case 1:
                printf("Введите фамилию: ");
                scanf("%s", temp_contact.surname);
                printf("Введите имя: ");
                scanf("%s", temp_contact.name);
                printf("Введите отчество (или -): ");
                scanf("%s", temp_contact.middle_name);
                printf("Введите компанию (или -): ");
                scanf("%s", temp_contact.company);
                printf("Введите должность (или -): ");
                scanf("%s", temp_contact.job_position);
                printf("Введите номер телефона (или -): ");
                scanf("%s", temp_contact.phone_number);
                printf("Введите email (или -): ");
                scanf("%s", temp_contact.email);
                printf("Введите социальную сеть (или -): ");
                scanf("%s", temp_contact.social_link);
                printf("Введите профиль мессенджера (или -): ");
                scanf("%s", temp_contact.messenger_profile);

                add_contact(contacts, &count, temp_contact);
                printf("Контакт добавлен.\n");
                break;

            case 2:
                printf("Введите номер контакта для редактирования: ");
                if (scanf("%d", &index) != 1 || index < 1 || index > count) { // Проверяем корректность ввода
                    printf("Некорректный номер контакта. Попробуйте снова.\n");
                    while (getchar() != '\n'); // Очищаем буфер ввода
                    break;
                }

	   printf("Введите фамилию: ");
                scanf("%s", temp_contact.surname);
                printf("Введите имя: ");
                scanf("%s", temp_contact.name);
                printf("Введите отчество (или -): ");
                scanf("%s", temp_contact.middle_name);
                printf("Введите компанию (или -): ");
                scanf("%s", temp_contact.company);
                printf("Введите должность (или -): ");
                scanf("%s", temp_contact.job_position);
                printf("Введите номер телефона (или -): ");
                scanf("%s", temp_contact.phone_number);
                printf("Введите email (или -): ");
                scanf("%s", temp_contact.email);
                printf("Введите социальную сеть (или -): ");
                scanf("%s", temp_contact.social_link);
                printf("Введите профиль мессенджера (или -): ");
                scanf("%s", temp_contact.messenger_profile);

                edit_contact(&contacts[index - 1], temp_contact);
                printf("Контакт отредактирован.\n");
                break;

            case 3:
                if (count == 0) {
                    printf("Список контактов пуст. Удаление невозможно.\n");
                    break;
                }

                printf("Введите номер контакта для удаления (1-%d): ", count);
                if (scanf("%d", &index) != 1 || index < 1 || index > count) { // Проверяем корректность ввода
                    printf("Некорректный номер контакта. Попробуйте снова.\n");
                    while (getchar() != '\n'); // Очищаем буфер ввода
                    break;
                }

                delete_contact(contacts, &count, index - 1);
                printf("Контакт удалён.\n");
                break;

            case 4:
                print_contacts(contacts, count);
                break;

            case 5:
                save_contacts(NEW_FILENAME, contacts, count);
                printf("Контакты сохранены файл \"%s\".\n", NEW_FILENAME);
                printf("Выход из программы...\n");
                exit(0);

            default:
                printf("Некорректный выбор. Попробуйте снова.\n");
                while (getchar() != '\n'); // Очищаем буфер ввода
                break;
        }
    }

    return 0;
}
