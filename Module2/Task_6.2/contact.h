#ifndef CONTACT_H
#define CONTACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Contact {
    char surname[50];
    char name[50];
    char middle_name[50];
    char company[50];
    char job_position[50];
    char phone_number[20];
    char email[50];
    char social_link[100];
    char messenger_profile[100];
    struct Contact *prev;
    struct Contact *next;
} Contact;

#define SUCCESS 0
#define FILE_ERROR 1
#define NOT_FOUND 2

// Функции для работы с контактами
void initialize_locale();
Contact* load_contacts(int *status);
int save_contacts(Contact *head);
void add_contact(Contact **head, Contact *new_contact);
int edit_contact(Contact *head, int index, const Contact *updated_contact);
int delete_contact(Contact **head, int index);
void free_contacts(Contact *head);
Contact* display_contacts(Contact *head);
#endif // CONTACT_H