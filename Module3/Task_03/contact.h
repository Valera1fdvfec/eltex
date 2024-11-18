#ifndef CONTACT_H
#define CONTACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_CONTACTS 100

typedef struct {
    char surname[50];
    char name[50];
    char middle_name[50];
    char company[50];
    char job_position[50];
    char phone_number[20];
    char email[50];
    char social_link[100];
    char messenger_profile[100];
} Contact;

// Функции для работы с контактами
void load_contacts(const char *filename, Contact *contacts, int *count);
void save_contacts(const char *filename, Contact *contacts, int count);
void add_contact(Contact *contacts, int *count, Contact new_contact);
void edit_contact(Contact *contact, Contact updated_contact);
void delete_contact(Contact *contacts, int *count, int index);

#endif
