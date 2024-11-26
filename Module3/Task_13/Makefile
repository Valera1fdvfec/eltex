CC = gcc
CFLAGS = -Wall -g

all: parent_psx child_psx

parent_psx: parent_psx.c shared_memory_psx.c
	$(CC) $(CFLAGS) -o parent_psx parent_psx.c shared_memory_psx.c -lrt -pthread

child_psx: child_psx.c shared_memory_psx.c
	$(CC) $(CFLAGS) -o child_psx child_psx.c shared_memory_psx.c -lrt -pthread

clean:
	rm -f parent_psx child_psx
	rm -f /dev/shm/posix_shared_memory
	rm -f /dev/shm/sem.sem_data_ready /dev/shm/sem.sem_result_ready
