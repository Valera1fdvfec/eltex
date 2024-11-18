# Указываем компилятор
CC = gcc

# Опции компиляции
CFLAGS = -Wall -Wextra -std=c99

# Имя исполняемого файла
TARGET = main

# Исходные файлы
SRCS = main.c calculate_areas.c
OBJS = $(SRCS:.c=.o)

# Заголовочные файлы
HEADERS = calculate_areas.h

# Правило по умолчанию
all: $(TARGET)

# Правило для компиляции исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Правило для компиляции объектных файлов
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило очистки сгенерированных файлов
clean:
	rm -f $(TARGET) $(OBJS)
