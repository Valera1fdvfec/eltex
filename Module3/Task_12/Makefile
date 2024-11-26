CC = gcc
CFLAGS = -Wall -g

all: parent child

parent: parent.c shared_memory.c
	$(CC) $(CFLAGS) -o parent parent.c shared_memory.c

child: child.c shared_memory.c
	$(CC) $(CFLAGS) -o child child.c shared_memory.c

clean:
	rm -f parent child
