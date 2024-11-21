CC = gcc
CFLAGS = -Wall -g -pthread

OBJ = main_two.o semaphore_two.o
TARGET = semaphore_two

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main_two.o: main_two.c semaphore_two.h
	$(CC) $(CFLAGS) -c main_two.c

semaphore_two.o: semaphore_two.c semaphore_two.h
	$(CC) $(CFLAGS) -c semaphore_two.c

clean:
	rm -f $(OBJ) $(TARGET)
