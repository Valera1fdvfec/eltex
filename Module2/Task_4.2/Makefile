CC = gcc
CFLAGS = -Wall -g

# Название исполняемого файла
TARGET = priority_program

# Исходные файлы
SRC = main.c priority.c
HEADER = priority.h

# Компиляция исполняемого файла
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Очистка скомпилированных файлов
clean:
	rm -f $(TARGET)
