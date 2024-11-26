CC = gcc
CFLAGS = -Wall -g
OBJ_CLIENT = TCP_client_files.o
OBJ_SERVER = TCP_server_files.o
TARGETS = TCP_client_files TCP_server_files

all: $(TARGETS)

TCP_client_files: $(OBJ_CLIENT)
	$(CC) $(CFLAGS) -o TCP_client_files $(OBJ_CLIENT)

TCP_server_files: $(OBJ_SERVER)
	$(CC) $(CFLAGS) -o TCP_server_files $(OBJ_SERVER)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
