CC = gcc
CFLAGS = -Wall

all: cli driver

cli: main.o get_status.o get_drivers.o create_drivers.o send_task.o
	$(CC) $(CFLAGS) -o cli main.o get_status.o get_drivers.o create_drivers.o send_task.o

driver: driver.o
	$(CC) $(CFLAGS) -o driver driver.o

main.o: main.c common.h get_status.h get_drivers.h create_drivers.h send_task.h
	$(CC) $(CFLAGS) -c main.c

get_status.o: get_status.c get_status.h common.h
	$(CC) $(CFLAGS) -c get_status.c

get_drivers.o: get_drivers.c get_drivers.h common.h get_status.h
	$(CC) $(CFLAGS) -c get_drivers.c

create_drivers.o: create_drivers.c create_drivers.h common.h
	$(CC) $(CFLAGS) -c create_drivers.c

send_task.o: send_task.c send_task.h common.h get_status.h
	$(CC) $(CFLAGS) -c send_task.c

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

clean:
	rm -f *.o cli driver
