CC = gcc
CFLAGS = -Wall

all: server_mult client_mult

server_mult: TCP_server_mult.o
	$(CC) $(CFLAGS) -o server_mult TCP_server_mult.o

client_mult: TCP_client_mult.o
	$(CC) $(CFLAGS) -o client_mult TCP_client_mult.o

TCP_server_mult.o: TCP_server_mult.c
	$(CC) $(CFLAGS) -c TCP_server_mult.c

TCP_client_mult.o: TCP_client_mult.c
	$(CC) $(CFLAGS) -c TCP_client_mult.c

clean:
	rm -f server_mult client_mult *.o
