CC = gcc
CFLAGS = -Wall -std=c99
OBJ = chat.o

all: main1 main2

main1: main1.c $(OBJ)
	$(CC) $(CFLAGS) -o main1 main1.c $(OBJ)

main2: main2.c $(OBJ)
	$(CC) $(CFLAGS) -o main2 main2.c $(OBJ)

chat.o: chat.c chat.h
	$(CC) $(CFLAGS) -c chat.c

clean:
	rm -f *.o main1 main2
