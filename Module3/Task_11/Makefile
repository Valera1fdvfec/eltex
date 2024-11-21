CC = gcc
CFLAGS = -Wall -g -pthread

OBJ = main_psx2.o semaphore_psx2.o
TARGET = semaphore_psx2

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main_psx2.o: main_psx2.c semaphore_psx2.h
	$(CC) $(CFLAGS) -c main_psx2.c

semaphore_psx2.o: semaphore_psx2.c semaphore_psx2.h
	$(CC) $(CFLAGS) -c semaphore_psx2.c

clean:
	rm -f $(OBJ) $(TARGET)
