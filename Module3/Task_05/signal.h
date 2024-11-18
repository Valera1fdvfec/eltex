#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

extern volatile sig_atomic_t access_allowed;
extern volatile sig_atomic_t child_done;

// Функции для работы с каналами
void child_process(int pipe_fd, const char *file);
void parent_process(int read_fd, int count, pid_t child_pid);
void handle_signal(int sig);

#endif // SIGNAL_H
