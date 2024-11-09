#include "bintree_contact.h"
#include <stdio.h>
#include <string.h>

// Функция для ввода данных контакта с консоли
void input_contact(Contact *contact) {
    do {
        printf("Введите фамилию (обязательно): ");
        fgets(contact->surname, sizeof(contact->surname), stdin);
        contact->surname[strcspn(contact->surname, "\n")] = '\0';
    } while (strlen(contact->surname) == 0);

    do {
        printf("Введите имя (обязательно): ");
        fgets(contact->name, sizeof(contact->name), stdin);
        contact->name[strcspn(contact->name, "\n")] = '\0';
    } while (strlen(contact->name) == 0);

    printf("Введите отчество (оставьте пустым для '-'): ");
    fgets(contact->middle_name, sizeof(contact->middle_name), stdin);
    contact->middle_name[strcspn(contact->middle_name, "\n")] = '\0';
    if (strlen(contact->middle_name) == 0) {
        strcpy(contact->middle_name, "-");
    }

    printf("Введите место работы (оставьте пустым для '-'): ");
    fgets(contact->company, sizeof(contact->company), stdin);
    contact->company[strcspn(contact->company, "\n")] = '\0';
    if (strlen(contact->company) == 0) {
        strcpy(contact->company, "-");
    }

    printf("Введите должность (оставьте пустым для '-'): ");
    fgets(contact->job_position, sizeof(contact->job_position), stdin);
    contact->job_position[strcspn(contact->job_position, "\n")] = '\0';
    if (strlen(contact->job_position) == 0) {
        strcpy(contact->job_position, "-");
    }

    printf("Введите номер телефона (оставьте пустым для '-'): ");
    fgets(contact->phone_number, sizeof(contact->phone_number), stdin);
    contact->phone_number[strcspn(contact->phone_number, "\n")] = '\0';
    if (strlen(contact->phone_number) == 0) {
        strcpy(contact->phone_number, "-");
    }

    printf("Введите адрес электронной почты (оставьте пустым для '-'): ");
    fgets(contact->email, sizeof(contact->email), stdin);
    contact->email[strcspn(contact->email, "\n")] = '\0';
    if (strlen(contact->email) == 0) {
        strcpy(contact->email, "-");
    }

    printf("Введите ссылку на соцсети (оставьте пустым для '-'): ");
    fgets(contact->social_link, sizeof(contact->social_link), stdin);
    contact->social_link[strcspn(contact->social_link, "\n")] = '\0';
    if (strlen(contact->social_link) == 0) {
        strcpy(contact->social_link, "-");
    }

    printf("Введите профиль в мессенджере (оставьте пустым для '-'): ");
    fgets(contact->messenger_profile, sizeof(contact->messenger_profile), stdin);
    contact->messenger_profile[strcspn(contact->messenger_profile, "\n")] = '\0';
    if (strlen(contact->messenger_profile) == 0) {
        strcpy(contact->messenger_profile, "-");
    }
}

// Функция для вывода полной информации о контакте
void print_contact_info(const Contact *contact, int contact_number) {
    printf("Контакт %d:\n", contact_number);
    printf("%s %s %s\n", contact->surname, contact->name, contact->middle_name);
    printf("%s\n", contact->company);
    printf("%s\n", contact->job_position);
    printf("%s\n", contact->phone_number);
    printf("%s\n", contact->email);
    printf("%s\n", contact->social_link);
    printf("%s\n\n", contact->messenger_profile);
}

// Функция для симметричного обхода и отображения всех контактов
void display_contacts(TreeNode *root, int *contact_number) {
    if (root == NULL) return;

    display_contacts(root->left, contact_number);
    print_contact_info(&root->contact, (*contact_number)++);
    display_contacts(root->right, contact_number);
}

int main() {
    initialize_locale();
    TreeNode *root = load_contacts();
    int choice;

    while (1) {
        printf("\nМеню:\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать контакты\n");
        printf("3. Найти контакт\n");
        printf("4. Удалить контакт\n");
        printf("5. Редактировать контакт\n");
        printf("6. Балансировка дерева\n");
        printf("7. Выйти\n");
        printf("Выберите пункт: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 7) break;
	switch (choice) {
            case 1: {
                Contact new_contact;
                input_contact(&new_contact);
                root = insert_contact(root, &new_contact);
                save_contacts(root);
                printf("Контакт успешно добавлен.\n");
                break;
            }
            case 2:
		int contact_number = 1;
                display_contacts(root, &contact_number);
                break;
            case 3: {
                char surname[50];
                printf("Введите фамилию для поиска: ");
                fgets(surname, sizeof(surname), stdin);
                surname[strcspn(surname, "\n")] = '\0';
                
                TreeNode *result = find_contact(root, surname);
                if (result) {
                    print_contact_info(&result->contact, 1);
                } else {
                    printf("Контакт не найден.\n");
                }
                break;
            }
            case 4: {
                char surname[50];
                printf("Введите фамилию для удаления: ");
                fgets(surname, sizeof(surname), stdin);
                surname[strcspn(surname, "\n")] = '\0';

                TreeNode *result = find_contact(root, surname);
                if (result) {
                    root = delete_contact(root, surname);
                    save_contacts(root);
                    printf("Контакт успешно удален.\n");
                } else {
                    printf("Контакт с фамилией '%s' не найден.\n", surname);
                }
                break;
            }
            case 5: {
                char surname[50];
                printf("Введите фамилию контакта для редактирования: ");
                fgets(surname, sizeof(surname), stdin);
                surname[strcspn(surname, "\n")] = '\0';

                TreeNode *node_to_edit = find_contact(root, surname);
                if (node_to_edit == NULL) {
                    printf("Контакт не найден.\n");
                } else {
                    printf("Редактирование контакта:\n");
                    input_contact(&node_to_edit->contact);
                    save_contacts(root);
                    printf("Контакт успешно отредактирован и сохранен.\n");
                }
                break;
            }
            case 6:
                root = balance_tree(root);
                printf("Дерево сбалансировано.\n");
		print_tree(root, 0);
                save_contacts(root);
                break;
            default:
                printf("Неверный выбор.\n");
        }
    }

    free_tree(root);
    return 0;
}
