#ifndef CONTACT_H
#define CONTACT_H

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
void initialize_locale();
void load_contacts();
void save_contacts();
void add_contact();
void display_contacts();
void edit_contact();
void delete_contact();

#endif // CONTACT_H
