#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>

int busy_time = 0;

int main() {
    char buffer[128];
    fd_set read_fds;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(3, &read_fds); // CLI пишет в pipe, мы читаем с fd 3

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ret = select(4, &read_fds, NULL, NULL, &timeout);

        if (ret > 0 && FD_ISSET(3, &read_fds)) {
            int len = read(3, buffer, sizeof(buffer) - 1);
            if (len <= 0) break;
            buffer[len] = 0;

            if (strncmp(buffer, "TASK", 4) == 0) {
                int t;
                sscanf(buffer + 5, "%d", &t);
                busy_time = t;
            }
        }

        if (busy_time > 0) {
            sleep(1);
            busy_time--;
        }
    }

    return 0;
}
