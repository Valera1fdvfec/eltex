#include <stdio.h>
#include "contact.h"

void input_contact(Contact *contact) {
    printf("Введите фамилию (обязательно): ");
    fgets(contact->surname, 50, stdin);
    contact->surname[strcspn(contact->surname, "\n")] = '\0';

    printf("Введите имя (обязательно): ");
    fgets(contact->name, 50, stdin);
    contact->name[strcspn(contact->name, "\n")] = '\0';

    printf("Введите отчество (оставьте пустым для '-'): ");
    fgets(contact->middle_name, 50, stdin);
    contact->middle_name[strcspn(contact->middle_name, "\n")] = '\0';
    if (strlen(contact->middle_name) == 0) strcpy(contact->middle_name, "-");

    printf("Введите место работы (оставьте пустым для '-'): ");
    fgets(contact->company, 50, stdin);
    contact->company[strcspn(contact->company, "\n")] = '\0';
    if (strlen(contact->company) == 0) strcpy(contact->company, "-");
    
    printf("Введите должность (оставьте пустым для '-'): ");
    fgets(contact->job_position, 50, stdin);
    contact->job_position[strcspn(contact->job_position, "\n")] = '\0';
    if (strlen(contact->middle_name) == 0) strcpy(contact->middle_name, "-");

    printf("Введите номер телефона (оставьте пустым для '-'): ");
    fgets(contact->phone_number, 20, stdin);
    contact->phone_number[strcspn(contact->phone_number, "\n")] = '\0';
    if (strlen(contact->phone_number) == 0) strcpy(contact->phone_number, "-");

    printf("Введите адрес электронной почты (оставьте пустым для '-'): ");
    fgets(contact->email, 50, stdin);
    contact->email[strcspn(contact->email, "\n")] = '\0';
    if (strlen(contact->email) == 0) strcpy(contact->email, "-");

    printf("Введите ссылку на страницу в соцсетях (оставьте пустым для '-'): ");
    fgets(contact->social_link, 100, stdin);
    contact->social_link[strcspn(contact->social_link, "\n")] = '\0';
    if (strlen(contact->social_link) == 0) strcpy(contact->social_link, "-");

    printf("Введите профиль в мессенджере (оставьте пустым для '-'): ");
    fgets(contact->messenger_profile, 100, stdin);
    contact->messenger_profile[strcspn(contact->messenger_profile, "\n")] = '\0';
    if (strlen(contact->messenger_profile) == 0) strcpy(contact->messenger_profile, "-");
}

void print_contact(const Contact *contact, int index) {
    printf("\nКонтакт %d:\n", index);
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

int main() {
    initialize_locale();
    int status;
    Contact *head = load_contacts(&status);

    if (status == FILE_ERROR) {
        printf("Не удалось загрузить контакты из файла.\n");
    } else {
        printf("Контакты успешно загружены.\n");
    }

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
            case 1: {
                Contact new_contact;
                input_contact(&new_contact);
                add_contact(&head, &new_contact);
                if (save_contacts(head) == FILE_ERROR) {
                    printf("Ошибка при сохранении контактов.\n");
                } else {
                    printf("Контакт успешно добавлен!\n");
                }
                break;
            }
            case 2: {
                int index = 1;
                Contact *current = display_contacts(head);
                while (current != NULL) {
                    print_contact(current, index++);
                    current = current->next;
                }
                if (index == 1) {
                    printf("Контакты отсутствуют.\n");
                }
                break;
            }

	   case 3: {
		int index;
    		Contact *current;

    		// Цикл для повторного ввода номера контакта, если контакт не найден
    		while (1) {
        		printf("Введите номер контакта для редактирования: ");
        		scanf("%d", &index);
        		getchar();

        		// Проверка существования контакта без редактирования
        		current = head;
        		for (int i = 1; i < index && current != NULL; i++) {
            		current = current->next;
        	}

        	if (current == NULL) {
            		printf("Контакт не найден. Пожалуйста, введите корректный номер контакта.\n");
        	} else {
            		break;  // Выходим из цикла, если контакт найден
        	}
    	}

    		// Запрос данных для редактирования
    		Contact updated_contact;
    		input_contact(&updated_contact);
    		status = edit_contact(head, index, &updated_contact);
    
    		if (status == SUCCESS) {
			if (save_contacts(head) == FILE_ERROR) {
        			printf("Ошибка при сохранении контактов.\n");
    			} else {
        			printf("Контакт успешно отредактирован!\n");
			}
		} else {
		   printf("Не удалось отредактировать контакт.\n");
		}
    		break;
	}
            case 4: {
                int index;
                printf("Введите номер контакта для удаления: ");
                scanf("%d", &index);
                getchar();

                status = delete_contact(&head, index);
                if (status == NOT_FOUND) {
                    printf("Контакт не найден.\n");
                } else if (save_contacts(head) == FILE_ERROR) {
                    printf("Ошибка при сохранении контактов.\n");
                } else {
                    printf("Контакт успешно удален!\n");
                }
                break;
            }
            case 5:
                printf("Завершение программы.\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 5);

    free_contacts(head);
    return 0;
}
