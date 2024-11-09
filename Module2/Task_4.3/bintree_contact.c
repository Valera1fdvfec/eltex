#include "bintree_contact.h"
#include <locale.h>

// Инициализация локали для поддержки русского языка
void initialize_locale() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
}

// Создание нового узла дерева
TreeNode* create_node(const Contact *new_contact) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) {
        perror("Ошибка выделения памяти для узла дерева");
        exit(EXIT_FAILURE);
    }
    node->contact = *new_contact;
    node->left = node->right = NULL;
    return node;
}

// Функция добавления контакта в дерево
TreeNode* insert_contact(TreeNode *root, const Contact *new_contact) {
    if (root == NULL) {
        return create_node(new_contact);
    }
    if (strcmp(new_contact->surname, root->contact.surname) < 0) {
        root->left = insert_contact(root->left, new_contact);
    } else {
        root->right = insert_contact(root->right, new_contact);
    }
    return root;
}

// Функция поиска контакта по фамилии
TreeNode* find_contact(TreeNode *root, const char *surname) {
    if (root == NULL || strcmp(surname, root->contact.surname) == 0) {
        return root;
    }
    if (strcmp(surname, root->contact.surname) < 0) {
        return find_contact(root->left, surname);
    } else {
        return find_contact(root->right, surname);
    }
}

// Функция нахождения минимального элемента в дереве (для удаления)
TreeNode* find_min(TreeNode *node) {
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Функция удаления контакта из дерева
TreeNode* delete_contact(TreeNode *root, const char *surname) {
    if (root == NULL) return NULL;

    if (strcmp(surname, root->contact.surname) < 0) {
        root->left = delete_contact(root->left, surname);
    } else if (strcmp(surname, root->contact.surname) > 0) {
        root->right = delete_contact(root->right, surname);
    } else {
        // Узел найден
        if (root->left == NULL) {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }

        TreeNode *temp = find_min(root->right);
        root->contact = temp->contact;
        root->right = delete_contact(root->right, temp->contact.surname);
    }
    return root;
}

// Сохранение контактов в файл
int save_contacts(TreeNode *root) {
    FILE *file = fopen("updated_contacts.txt", "w");
    if (file == NULL) {
        perror("Ошибка при открытии файла 'updated_contacts.txt'");
        return FILE_ERROR;
    }

    void save_inorder(TreeNode *node) {
        if (node == NULL) return;
        save_inorder(node->left);
        fprintf(file, "%s %s %s %s %s %s %s %s %s\n",
                node->contact.surname, node->contact.name, node->contact.middle_name, node->contact.company,
                node->contact.job_position, node->contact.phone_number, node->contact.email,
                node->contact.social_link, node->contact.messenger_profile);
        save_inorder(node->right);
    }

    save_inorder(root);
    fclose(file);
    return SUCCESS;
}

// Загрузка контактов из файла и построение дерева
TreeNode* load_contacts() {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла 'contacts.txt'");
        return NULL;
    }

    TreeNode *root = NULL;
    Contact temp;
    while (fscanf(file, "%s %s %s %s %s %s %s %s %s",
                  temp.surname, temp.name, temp.middle_name, temp.company,
                  temp.job_position, temp.phone_number, temp.email,
                  temp.social_link, temp.messenger_profile) == 9) {
        root = insert_contact(root, &temp);
    }

    fclose(file);
    return root;
}

// Функция освобождения памяти, занятой деревом
void free_tree(TreeNode *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
// Функция для балансировки дерева
void store_inorder(TreeNode *root, Contact arr[], int *index) {
    if (root == NULL) return;
    store_inorder(root->left, arr, index);
    arr[(*index)++] = root->contact;
    store_inorder(root->right, arr, index);
}

TreeNode* build_balanced_tree(Contact arr[], int start, int end) {
    if (start > end) return NULL;

    int mid = (start + end) / 2;
    TreeNode *node = create_node(&arr[mid]);

    node->left = build_balanced_tree(arr, start, mid - 1);
    node->right = build_balanced_tree(arr, mid + 1, end);

    return node;
}

TreeNode* balance_tree(TreeNode *root) {
    int n = 0;
    Contact arr[5000]; // массив для хранения элементов дерева
    store_inorder(root, arr, &n);

    free_tree(root); // освобождаем старое дерево
    return build_balanced_tree(arr, 0, n - 1);
}

void print_tree(TreeNode *root, int space) {

    if (root == NULL) return;

    // Увеличение расстояния между уровнями
    space += 5;

    // Печать правого поддерева
    print_tree(root->right, space);

    // Печать текущего узла
    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%s\n", root->contact.surname);

    // Печать левого поддерева
    print_tree(root->left, space);
}
