#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "contact_3.h"

#define UPDATED_FILE "updated_contacts.txt"

Contact contacts[MAX_CONTACTS];
int contact_count = 0;

void initialize_locale() {
    setlocale(LC_ALL, "ru_RU.UTF-8"); // Устанавливаем локаль на русский язык
}

void load_contacts() {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("Не удалось открыть файл для чтения.\n");
        return;
    }

    while (fscanf(file, "%s %s %s %s %s %s %s %s %s",
                  contacts[contact_count].surname,
                  contacts[contact_count].name,
                  contacts[contact_count].middle_name,
                  contacts[contact_count].company,
                  contacts[contact_count].job_position,
                  contacts[contact_count].phone_number,
                  contacts[contact_count].email,
                  contacts[contact_count].social_link,
                  contacts[contact_count].messenger_profile) == 9) {
        contact_count++;
    }

    fclose(file);
    printf("Контакты успешно загружены из файла.\n");
}

void save_contacts() {
    FILE *file = fopen(UPDATED_FILE, "w");
    if (file == NULL) {
        printf("Не удалось открыть файл для записи обновленных данных.\n");
        return;
    }

    for (int i = 0; i < contact_count; i++) {
        fprintf(file, "%s %s %s %s %s %s %s %s %s\n",
                contacts[i].surname, contacts[i].name, contacts[i].middle_name, contacts[i].company,
                contacts[i].job_position, contacts[i].phone_number, contacts[i].email,
                contacts[i].social_link, contacts[i].messenger_profile);
    }
    fclose(file);
    printf("Обновленные данные успешно сохранены в файл '%s'.\n", UPDATED_FILE);
}

void input_or_default(char *field, const char *prompt) {
    printf("%s (оставьте пустым для '-'): ", prompt);
    fgets(field, 50, stdin);
    field[strcspn(field, "\n")] = '\0';
    if (strlen(field) == 0) {
        strcpy(field, "-");
    }
}

void add_contact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("Невозможно добавить больше контактов. Список заполнен.\n");
        return;
    }

    Contact new_contact;
    printf("Введите фамилию (обязательно): ");
    fgets(new_contact.surname, 50, stdin);
    new_contact.surname[strcspn(new_contact.surname, "\n")] = '\0';

    printf("Введите имя (обязательно): ");
    fgets(new_contact.name, 50, stdin);
    new_contact.name[strcspn(new_contact.name, "\n")] = '\0';

    printf("Введите отчество (оставьте пустым для '-'): ");
    fgets(new_contact.middle_name, 50, stdin);
    new_contact.middle_name[strcspn(new_contact.middle_name, "\n")] = '\0';
    if (strlen(new_contact.middle_name) == 0) strcpy(new_contact.middle_name, "-");

    input_or_default(new_contact.company, "Введите место работы");
    input_or_default(new_contact.job_position, "Введите должность");
    input_or_default(new_contact.phone_number, "Введите номер телефона");
    input_or_default(new_contact.email, "Введите адрес электронной почты");
    input_or_default(new_contact.social_link, "Введите ссылку на страницу в соцсетях");
    input_or_default(new_contact.messenger_profile, "Введите профиль в мессенджере");

    contacts[contact_count++] = new_contact;
    printf("Контакт успешно добавлен!\n");

    save_contacts();
}



void display_contacts() {
    if (contact_count == 0) {
        printf("Контакты отсутствуют.\n");
        return;
    }
    for (int i = 0; i < contact_count; i++) {
        printf("\nКонтакт %d:\n", i + 1);
        printf("Фамилия: %s\n", contacts[i].surname);
        printf("Имя: %s\n", contacts[i].name);
        printf("Отчество: %s\n", contacts[i].middle_name);
        printf("Компания: %s\n", contacts[i].company);
        printf("Должность: %s\n", contacts[i].job_position);
        printf("Номер телефона: %s\n", contacts[i].phone_number);
        printf("Электронная почта: %s\n", contacts[i].email);
        printf("Ссылка на соцсети: %s\n", contacts[i].social_link);
        printf("Профиль в мессенджере: %s\n", contacts[i].messenger_profile);
    }
}

void edit_contact() {
    int index;
    printf("Введите номер контакта для редактирования (1 до %d): ", contact_count);
    scanf("%d", &index);
    getchar();
    index--;

    if (index < 0 || index >= contact_count) {
        printf("Неверный номер контакта.\n");
        return;
    }

    printf("Редактирование контакта %d:\n", index + 1);
    printf("Введите фамилию (обязательно): ");
    fgets(contacts[index].surname, 50, stdin);
    contacts[index].surname[strcspn(contacts[index].surname, "\n")] = '\0';

    printf("Введите имя (обязательно): ");
    fgets(contacts[index].name, 50, stdin);
    contacts[index].name[strcspn(contacts[index].name, "\n")] = '\0';

    printf("Введите отчество (оставьте пустым для '-'): ");
    fgets(contacts[index].middle_name, 50, stdin);
    contacts[index].middle_name[strcspn(contacts[index].middle_name, "\n")] = '\0';
    if (strlen(contacts[index].middle_name) == 0) strcpy(contacts[index].middle_name, "-");

    input_or_default(contacts[index].company, "Введите место работы");
    input_or_default(contacts[index].job_position, "Введите должность");
    input_or_default(contacts[index].phone_number, "Введите номер телефона");
    input_or_default(contacts[index].email, "Введите адрес электронной почты");
    input_or_default(contacts[index].social_link, "Введите ссылку на страницу в соцсетях");
    input_or_default(contacts[index].messenger_profile, "Введите профиль в мессенджере");

    printf("Контакт успешно отредактирован!\n");
    save_contacts();
}

void delete_contact() {
    int index;
    printf("Введите номер контакта для удаления (1 до %d): ", contact_count);
    scanf("%d", &index);
    getchar();
    index--;

    if (index < 0 || index >= contact_count) {
        printf("Неверный номер контакта.\n");
        return;
    }

    for (int i = index; i < contact_count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contact_count--;
    printf("Контакт успешно удален!\n");
    save_contacts();
}
