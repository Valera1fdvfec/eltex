#ifndef GET_STATUS_H
#define GET_STATUS_H

#include "common.h"

DriverInfo* find_driver(int pid);
void update_status(DriverInfo* d);
void get_status(int pid);

#endif
