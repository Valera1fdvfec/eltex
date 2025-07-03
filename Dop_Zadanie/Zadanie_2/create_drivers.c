#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "create_drivers.h"

DriverInfo drivers[MAX_DRIVERS];
int driver_count = 0;

void create_driver(void) {
    if (driver_count >= MAX_DRIVERS) {
        printf("Максимум драйверов достигнут\n");
        return;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        close(pipe_fd[1]); 
        if (dup2(pipe_fd[0], 3) == -1) {
            perror("dup2");
            exit(1);
        }
        close(pipe_fd[0]);
        if (execl("./driver", "driver", NULL) == -1) {
            perror("execl oshibka");
            exit(1);
        }
        exit(1);
    }

    drivers[driver_count].pid = pid;
    drivers[driver_count].status = AVAILABLE;
    drivers[driver_count].pipe_fd[0] = pipe_fd[0];
    drivers[driver_count].pipe_fd[1] = pipe_fd[1];
    drivers[driver_count].task_end_time = 0;
    driver_count++;

    printf("Создан driver с PID: %d\n", pid);
}
