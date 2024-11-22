CC = gcc
CFLAGS = -Wall

all: server client1 client2

server: UDP_server.o
	$(CC) $(CFLAGS) -o server UDP_server.o

client1: UDP_client1.o
	$(CC) $(CFLAGS) -o client1 UDP_client1.o

client2: UDP_client2.o
	$(CC) $(CFLAGS) -o client2 UDP_client2.o

UDP_server.o: UDP_server.c
	$(CC) $(CFLAGS) -c UDP_server.c

UDP_client1.o: UDP_client1.c
	$(CC) $(CFLAGS) -c UDP_client1.c

UDP_client2.o: UDP_client2.c
	$(CC) $(CFLAGS) -c UDP_client2.c

clean:
	rm -f server client1 client2 *.o
