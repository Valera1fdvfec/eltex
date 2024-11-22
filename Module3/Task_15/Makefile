CC = gcc
CFLAGS = -Wall -g
OBJ_CLIENT = TCP_client.o
OBJ_SERVER = TCP_server.o
TARGETS = TCP_client TCP_server

all: $(TARGETS)

TCP_client: $(OBJ_CLIENT)
	$(CC) $(CFLAGS) -o TCP_client $(OBJ_CLIENT)

TCP_server: $(OBJ_SERVER)
	$(CC) $(CFLAGS) -o TCP_server $(OBJ_SERVER)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
