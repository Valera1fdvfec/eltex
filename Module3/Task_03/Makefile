CC = gcc
CFLAGS = -Wall -g

OBJ = main.o contact.o
TARGET = contact

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c contact.h
	$(CC) $(CFLAGS) -c main.c

contact.o: contact.c contact.h
	$(CC) $(CFLAGS) -c contact.c

clean:
	rm -f $(OBJ) $(TARGET)
