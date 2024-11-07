CC = gcc
CFLAGS = -Wall -g
TARGET = contacts

all: $(TARGET)

$(TARGET): main.c contact.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c contact.c

clean:
	rm -f $(TARGET) * .o
