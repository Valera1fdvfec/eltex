#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "get_status.h"

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
