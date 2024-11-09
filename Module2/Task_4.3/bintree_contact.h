#ifndef CONTACT_H
#define CONTACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для представления контакта
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
} Contact;

#define SUCCESS 0
#define FILE_ERROR 1
#define NOT_FOUND 2


// Узел бинарного дерева для контакта
typedef struct TreeNode {
    Contact contact;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Функции для работы с контактами
void initialize_locale();
TreeNode* insert_contact(TreeNode *root, const Contact *new_contact);
TreeNode* find_contact(TreeNode *root, const char *surname);
TreeNode* delete_contact(TreeNode *root, const char *surname);
void display_contacts(TreeNode *root, int *contact_number);
void free_tree(TreeNode *root);
TreeNode* balance_tree(TreeNode *root);

int save_contacts(TreeNode *root);
TreeNode* load_contacts();
void inorder_traversal(TreeNode *root, void (*func)(Contact *));
void print_tree(TreeNode *root, int space);
#endif // CONTACT_H
