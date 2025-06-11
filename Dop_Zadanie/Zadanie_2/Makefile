# Makefile

CC = gcc
CFLAGS = -Wall

all: cli driver

cli: cli_tax.c
	$(CC) $(CFLAGS) -o cli_tax cli_tax.c

driver: driver.c
	$(CC) $(CFLAGS) -o driver driver.c

clean:
	rm -f cli driver
