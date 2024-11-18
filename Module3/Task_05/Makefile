CC = gcc
CFLAGS = -Wall -g

OBJ = main.o signal.o
TARGET = signal

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c signal.h
	$(CC) $(CFLAGS) -c main.c

signal.o: signal.c signal.h
	$(CC) $(CFLAGS) -c signal.c

clean:
	rm -f $(OBJ) $(TARGET)
