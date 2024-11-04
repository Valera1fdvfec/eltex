#ifndef FILE_PERMISSIONS_H
#define FILE_PERMISSIONS_H

#include <sys/types.h>
#include <sys/stat.h>

// Объявления функций
void displayPermissions(mode_t mode);
void displayBinaryPermissions(mode_t mode);
void getPermissions(char *filename);
void setPermissions(char *filename, mode_t mode);

#endif // FILE_PERMISSIONS_H
