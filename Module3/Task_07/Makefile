CC = gcc
CFLAGS = -Wall -std=c99
OBJ = chat_posix.o

all: main1 main2

main1: main1.c $(OBJ)
	$(CC) $(CFLAGS) -o main1 main1.c $(OBJ) -lrt

main2: main2.c $(OBJ)
	$(CC) $(CFLAGS) -o main2 main2.c $(OBJ) -lrt

chat_posix.o: chat_posix.c chat_posix.h
	$(CC) $(CFLAGS) -c chat_posix.c

clean:
	rm -f *.o main1 main2
