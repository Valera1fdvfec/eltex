#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "send_task.h"

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
