# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -g

# Список объектных файлов
OBJECTS = server.o server_main.o client1.o client_main.o raw_socket.o

# Имена исполняемых файлов
SERVER = server
CLIENT1 = client

# Основное правило: компиляция сервера и клиента
all: $(SERVER) $(CLIENT1)

# Правило для сборки сервера
$(SERVER): server.o server_main.o raw_socket.o
	$(CC) -o $(SERVER) server.o server_main.o raw_socket.o

# Правило для сборки клиента
$(CLIENT1): client1.o client_main.o raw_socket.o
	$(CC) -o $(CLIENT1) client1.o client_main.o raw_socket.o

# Правила компиляции объектных файлов
server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c

server_main.o: server_main.c server.h
	$(CC) $(CFLAGS) -c server_main.c

client1.o: client1.c server.h
	$(CC) $(CFLAGS) -c client1.c

client_main.o: client_main.c server.h
	$(CC) $(CFLAGS) -c client_main.c

raw_socket.o: raw_socket.c server.h
	$(CC) $(CFLAGS) -c raw_socket.c

# Правило для очистки: удаление всех объектных файлов
clean:
	rm -f *.o

# Правило для очистки всего, включая исполняемые файлы
distclean: clean
	rm -f $(SERVER) $(CLIENT1)
