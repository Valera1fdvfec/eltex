CC = gcc
CFLAGS = -Wall -g -pthread

OBJ = main.o semaphore_psx.o
TARGET = semaphore_psx

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c semaphore_psx.h
	$(CC) $(CFLAGS) -c main.c

semaphore_psx.o: semaphore_psx.c semaphore_psx.h
	$(CC) $(CFLAGS) -c semaphore_psx.c

clean:
	rm -f $(OBJ) $(TARGET)
