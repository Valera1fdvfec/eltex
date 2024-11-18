#ifndef KANAL_H
#define KANAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Функции для работы с каналами
void generate_numbers(int write_fd, int count);
void parent_process(int read_fd, int count);

#endif // KANAL_H
