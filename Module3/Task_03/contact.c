#include "contact.h"

#define BUFFER_SIZE 1024

void load_contacts(const char *filename, Contact *contacts, int *count) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        *count = 0;
        perror("Ошибка открытия файла для чтения");
        return;
    }

    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    int line_index = 0;
    ssize_t bytes_read;
    *count = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                // Завершаем строку
                line[line_index] = '\0';

                // Обрабатываем строку
                if (*count < MAX_CONTACTS) {
                    Contact *contact = &contacts[*count];
                    int parsed_fields = sscanf(line, "%49s %49s %49s %49s %49s %19s %49s %99s %99s",
                                               contact->surname, contact->name, contact->middle_name, contact->company,
                                               contact->job_position, contact->phone_number, contact->email,
                                               contact->social_link, contact->messenger_profile);

                    if (parsed_fields == 9) {
                        (*count)++;
                    } else {
                        fprintf(stderr, "Ошибка: некорректный формат строки: %s\n", line);
                    }
                }

                // Сбрасываем индекс строки
                line_index = 0;
            } else {
                // Добавляем символ в строку
                if (line_index < BUFFER_SIZE - 1) {
                    line[line_index++] = buffer[i];
                } else {
                    fprintf(stderr, "Ошибка: слишком длинная строка\n");
                    line_index = 0; // Сбрасываем строку при ошибке
                }
            }
        }
    }

    if (bytes_read == -1) {
        perror("Ошибка чтения файла");
    }

    // Проверяем, есть ли незавершённая строка
    if (line_index > 0) {
        line[line_index] = '\0';
        if (*count < MAX_CONTACTS) {
            Contact *contact = &contacts[*count];
            int parsed_fields = sscanf(line, "%49s %49s %49s %49s %49s %19s %49s %99s %99s",
                                       contact->surname, contact->name, contact->middle_name, contact->company,
                                       contact->job_position, contact->phone_number, contact->email,
                                       contact->social_link, contact->messenger_profile);

            if (parsed_fields == 9) {
                (*count)++;
            } else {
                fprintf(stderr, "Ошибка: некорректный формат строки: %s\n", line);
            }
        }
    }

    if (close(fd) == -1) {
        perror("Ошибка закрытия файла");
    }
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

