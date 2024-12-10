# Компилятор
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Wextra -lpthread -Wno-unused-parameter

# Исполняемые файлы
TARGETS = server client1 client2 sniffer

# Исходные файлы
SERVER_SRC = UDP_server.c
CLIENT1_SRC = UDP_client1.c
CLIENT2_SRC = UDP_client2.c
SNIFFER_SRC = RAW_sniffer.c

# Исполняемые файлы и их зависимости
all: $(TARGETS)

server: $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

client1: $(CLIENT1_SRC)
	$(CC) $(CFLAGS) -o $@ $^

client2: $(CLIENT2_SRC)
	$(CC) $(CFLAGS) -o $@ $^

sniffer: $(SNIFFER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Удаление всех скомпилированных файлов
clean:
	rm -f $(TARGETS) *.o

# Рекомпиляция всех файлов
rebuild: clean all
