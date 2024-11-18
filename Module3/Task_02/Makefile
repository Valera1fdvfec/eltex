# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Имя исполняемого файла
TARGET = my_shell

# Исходные и объектные файлы
SRCS = my_shell.c main.c
OBJS = $(SRCS:.c=.o)

# Заголовочный файл
HEADERS = my_shell.h

# Правило по умолчанию
all: $(TARGET)

# Правило сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Правило для создания объектных файлов
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(TARGET) $(OBJS)
