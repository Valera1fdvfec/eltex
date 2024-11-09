#include <locale.h>
#include "contact.h"

void initialize_locale() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
}

Contact* load_contacts(int *status) {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        *status = FILE_ERROR;
        return NULL;
    }

    Contact *head = NULL;
    Contact temp;

    while (fscanf(file, "%s %s %s %s %s %s %s %s %s",
                  temp.surname, temp.name, temp.middle_name, temp.company,
                  temp.job_position, temp.phone_number, temp.email,
                  temp.social_link, temp.messenger_profile) == 9) {
        add_contact(&head, &temp);
    }

    fclose(file);
    *status = SUCCESS;
    return head;
}

int save_contacts(Contact *head) {
    FILE *file = fopen("updated_contacts.txt", "w");
    if (file == NULL) {
        return FILE_ERROR;
    }

    Contact *current = head;
    while (current) {
        int result = fprintf(file, "%s %s %s %s %s %s %s %s %s\n",
                current->surname, current->name, current->middle_name, current->company,
                current->job_position, current->phone_number, current->email,
                current->social_link, current->messenger_profile);
        if (result < 0) {
		perror ("Oшибка при зaписи в файл.\n");
		fclose(file);
    		return FILE_ERROR;
	}
	current = current->next;
    }

    fclose(file);
    return SUCCESS;
}

void add_contact(Contact **head, Contact *new_contact) {
    // Новый элемент списка
    Contact *new_node = (Contact *)malloc(sizeof(Contact));
    if (new_node == NULL) {
        perror("Ошибка выделения памяти для нового контакта");
        return;
    }
    *new_node = *new_contact;  // Копируем данные контакта

    new_node->next = NULL;
    new_node->prev = NULL;

    if (*head == NULL) {
        // Если список пуст, новый элемент становится головой списка
        *head = new_node;
    } else {
        // Вставка в конец списка
        Contact *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
        new_node->prev = current;
    }
}

int edit_contact(Contact *head, int index, const Contact *updated_contact) {
    Contact *current = head;

    for (int i = 1; i < index && current != NULL; i++) {
      current = current->next;
    }

    if (current == NULL) {
	return NOT_FOUND;
    } 

    // Обновляем обязательные поля
    strncpy(current->surname, updated_contact->surname, sizeof(current->surname) - 1);
    current->surname[sizeof(current->surname) - 1] = '\0';

    strncpy(current->name, updated_contact->name, sizeof(current->name) - 1);
    current->name[sizeof(current->name) - 1] = '\0';

    // Обновляем необязательные поля или заполняем дефисом, если они пусты
    if (strlen(updated_contact->middle_name) == 0) {
        strcpy(current->middle_name, "-");
    } else {
        strncpy(current->middle_name, updated_contact->middle_name, sizeof(current->middle_name) - 1);
        current->middle_name[sizeof(current->middle_name) - 1] = '\0';
    }

    if (strlen(updated_contact->company) == 0) {
        strcpy(current->company, "-");
    } else {
        strncpy(current->company, updated_contact->company, sizeof(current->company) - 1);
        current->company[sizeof(current->company) - 1] = '\0';
    }

    if (strlen(updated_contact->job_position) == 0) {
        strcpy(current->job_position, "-");
    } else {
        strncpy(current->job_position, updated_contact->job_position, sizeof(current->job_position) - 1);
        current->job_position[sizeof(current->job_position) - 1] = '\0';
    }

    if (strlen(updated_contact->phone_number) == 0) {
        strcpy(current->phone_number, "-");
    } else {
        strncpy(current->phone_number, updated_contact->phone_number, sizeof(current->phone_number) - 1);
        current->phone_number[sizeof(current->phone_number) - 1] = '\0';
    }

    if (strlen(updated_contact->email) == 0) {
        strcpy(current->email, "-");
    } else {
        strncpy(current->email, updated_contact->email, sizeof(current->email) - 1);
        current->email[sizeof(current->email) - 1] = '\0';
    }

    if (strlen(updated_contact->social_link) == 0) {
        strcpy(current->social_link, "-");
    } else {
        strncpy(current->social_link, updated_contact->social_link, sizeof(current->social_link) - 1);
        current->social_link[sizeof(current->social_link) - 1] = '\0';
    }

    if (strlen(updated_contact->messenger_profile) == 0) {
        strcpy(current->messenger_profile, "-");
    } else {
        strncpy(current->messenger_profile, updated_contact->messenger_profile, sizeof(current->messenger_profile) - 1);
        current->messenger_profile[sizeof(current->messenger_profile) - 1] = '\0';
    }

    return SUCCESS;
}

int delete_contact(Contact **head, int index) {
    if (*head == NULL) {
        return NOT_FOUND;  // Список пуст
    }

    Contact *current = *head;

    // Перемещаемся до нужного элемента
    for (int i = 1; current != NULL && i < index; i++) {
        current = current->next;
    }

    if (current == NULL) {
        return NOT_FOUND;  // Элемент с таким индексом не найден
    }

    // Изменение связей для удаления элемента
    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        // Удаление головы списка
        *head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    }

    free(current);
    return SUCCESS;
}

void free_contacts(Contact *head) {
    while (head) {
        Contact *temp = head;
        head = head->next;
        free(temp);
    }
}
Contact* display_contacts(Contact* head) {
return head;
}
