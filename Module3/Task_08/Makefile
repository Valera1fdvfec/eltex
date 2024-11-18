CC = gcc
CFLAGS = -Wall -g -pthread

OBJ = main.o semaphore.o
TARGET = semaphore

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c semaphore.h
	$(CC) $(CFLAGS) -c main.c

semaphore.o: semaphore.c semaphore.h
	$(CC) $(CFLAGS) -c semaphore.c

clean:
	rm -f $(OBJ) $(TARGET)
