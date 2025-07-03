#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/wait.h>
#include "common.h"
#include "get_status.h"
#include "get_drivers.h"
#include "create_drivers.h"
#include "send_task.h"

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
