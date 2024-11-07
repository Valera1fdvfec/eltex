CC = gcc
CFLAGS = -Wall -g
TARGET = bintree_contacts

all: $(TARGET)

$(TARGET):main.c bintree_contact.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c bintree_contact.c

clean:
	rm -f $(TARGET) * .o
