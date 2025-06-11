// cli.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <time.h>
#include "common.h"

DriverInfo drivers[MAX_DRIVERS];
int driver_count = 0;

void create_driver();
void send_task(int pid, int time);
void get_status(int pid);
void get_drivers();
DriverInfo* find_driver(int pid);
void update_status(DriverInfo* d);

int main() {
    char command[MSG_SIZE];

    while (1) {
        printf("> ");
        fflush(stdout);
        if (!fgets(command, MSG_SIZE, stdin)) break;

        if (strncmp(command, "create_driver", 13) == 0) {
            create_driver();
        } else if (strncmp(command, "send_task", 9) == 0) {
            int pid, time;
            sscanf(command + 10, "%d %d", &pid, &time);
            send_task(pid, time);
        } else if (strncmp(command, "get_status", 10) == 0) {
            int pid;
            sscanf(command + 11, "%d", &pid);
            get_status(pid);
        } else if (strncmp(command, "get_drivers", 11) == 0) {
            get_drivers();
        }
    }

    return 0;
}

void create_driver() {
    if (driver_count >= MAX_DRIVERS) {
        printf("Максимум драйверов достигнут\n");
        return;
    }

    int pipe_fd[2];
    pipe(pipe_fd);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(pipe_fd[0], 3); // CLI будет писать в pipe_fd[1], драйвер читать с fd 3
        execl("./driver", "driver", NULL);
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

DriverInfo* find_driver(int pid) {
    for (int i = 0; i < driver_count; ++i)
        if (drivers[i].pid == pid)
            return &drivers[i];
    return NULL;
}

void update_status(DriverInfo* d) {
    if (d->status == BUSY && time(NULL) >= d->task_end_time) {
        d->status = AVAILABLE;
    }
}

void send_task(int pid, int task_time) {
    DriverInfo* d = find_driver(pid);
    if (!d) {
        printf("Driver не найден\n");
        return;
    }

    update_status(d);

    if (d->status == BUSY) {
        int remaining = (int)difftime(d->task_end_time, time(NULL));
        if (remaining < 0) remaining = 0;
        printf("Busy %d\n", remaining);
        return;
    }

    char msg[MSG_SIZE];
    snprintf(msg, sizeof(msg), "TASK %d\n", task_time);
    write(d->pipe_fd[1], msg, strlen(msg));
    d->status = BUSY;
    d->task_end_time = time(NULL) + task_time;

    printf("Задание отправлено driver %d на %d сек\n", pid, task_time);
}

void get_status(int pid) {
    DriverInfo* d = find_driver(pid);
    if (!d) {
        printf("Driver не найден\n");
        return;
    }
    update_status(d);

    if (d->status == BUSY) {
        int remaining = (int)difftime(d->task_end_time, time(NULL));
        if (remaining < 0) remaining = 0;
        printf("Driver %d статус: Busy %d\n", pid, remaining);
    } else {
        printf("Driver %d статус: Available\n", pid);
    }
}

void get_drivers() {
    for (int i = 0; i < driver_count; ++i) {
        update_status(&drivers[i]);
        if (drivers[i].status == BUSY) {
            int remaining = (int)difftime(drivers[i].task_end_time, time(NULL));
            if (remaining < 0) remaining = 0;
            printf("PID: %d Статус: Busy %d\n", drivers[i].pid, remaining);
        } else {
            printf("PID: %d Статус: Available\n", drivers[i].pid);
        }
    }
}
