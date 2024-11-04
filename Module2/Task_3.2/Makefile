TARGET = network_programm

CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Wextra -std=c99 
# Исходные файлы
SRCS = main.c network.c

# Заголовочные файлы
HEADERS = network.h

# Объектные файлы
OBJS = $(SRCS:.c=.o)

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Правило для компиляции объектных файлов
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для очистки скомпилированных файлов
clean:
	rm -f $(TARGET) $(OBJS)

# Файлы, которые не являются файлами
.PHONY: all clean
