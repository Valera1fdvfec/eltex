#ifndef COMMON_H
#define COMMON_H

#define MAX_DRIVERS 100
#define MSG_SIZE 128

typedef enum {
    AVAILABLE,
    BUSY
} DriverStatus;

typedef struct {
    int pid;
    DriverStatus status;
    int pipe_fd[2]; // [0] - read, [1] - write (cli <-> driver)
    int task_end_time; // seconds remaining
} DriverInfo;

// Глобальные переменные
extern DriverInfo drivers[MAX_DRIVERS];
extern int driver_count;

// Объявления функций
void create_driver(void);
void send_task(int pid, int time);
void get_status(int pid);
void get_drivers(void);
DriverInfo* find_driver(int pid);
void update_status(DriverInfo* d);

#endif
