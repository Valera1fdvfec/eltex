CC = gcc
CFLAGS = -Wall -g

OBJ = main.o kanal.o
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c kanal.h
	$(CC) $(CFLAGS) -c main.c

kanal.o: kanal.c kanal.h
	$(CC) $(CFLAGS) -c kanal.c

clean:
	rm -f $(OBJ) $(TARGET)
