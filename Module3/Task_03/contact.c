#include "contact.h"

void load_contacts(const char *filename, Contact *contacts, int *count) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        *count = 0;
        perror("Ошибка открытия файла для чтения");
        return;
    }

    char buffer[32768];
    int bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Ошибка чтения файла");
        close(fd);
        return;
    }

    buffer[bytes_read] = '\0'; // Добавляем конец строки
    char *line = strtok(buffer, "\n"); // Разбиваем на строки
    *count = 0;

    while (line != NULL && *count < MAX_CONTACTS) {
        Contact *contact = &contacts[*count];
        sscanf(line, "%49s %49s %49s %49s %49s %19s %49s %99s %99s",
               contact->surname, contact->name, contact->middle_name, contact->company,
               contact->job_position, contact->phone_number, contact->email,
               contact->social_link, contact->messenger_profile);
        (*count)++;
        line = strtok(NULL, "\n"); // Переходим к следующей строке
    }

    close(fd);
}

void save_contacts(const char *filename, Contact *contacts, int count) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Ошибка открытия файла для записи");
        return;
    }

    char buffer[32768];
    for (int i = 0; i < count; i++) {
        Contact *contact = &contacts[i];
        int length = snprintf(buffer, sizeof(buffer), "%s %s %s %s %s %s %s %s %s\n",
                              contact->surname, contact->name, contact->middle_name, contact->company,
                              contact->job_position, contact->phone_number, contact->email,
                              contact->social_link, contact->messenger_profile);

        if (write(fd, buffer, length) != length) {
            perror("Ошибка записи данных в файл");
            close(fd);
            return;
        }
    }

    close(fd);
}

void add_contact(Contact *contacts, int *count, Contact new_contact) {
    if (*count >= MAX_CONTACTS) {
        return;
    }
    contacts[*count] = new_contact;
    (*count)++;
}

void edit_contact(Contact *contact, Contact updated_contact) {
    *contact = updated_contact;
}

void delete_contact(Contact *contacts, int *count, int index) {
    if (index < 0 || index >= *count) {
        return;
    }
    for (int i = index; i < *count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    (*count)--;
}

