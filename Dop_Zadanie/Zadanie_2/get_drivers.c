#include <stdio.h>
#include <time.h>
#include "get_drivers.h"

void get_drivers(void) {
    if (driver_count == 0) {
        printf("Drivers пока не созданы.\n");
        return;
    }

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
