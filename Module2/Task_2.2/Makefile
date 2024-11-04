# Указываем компилятор
CC = gcc

# Опции компиляции
CFLAGS = -Wall -Wextra

# Имя исполняемого файла
TARGET = calculator

# Исходные файлы
SRCS = main.c calculator.c

# Заголовочные файлы
HEADERS = calculator.h

# Правило по умолчанию
all: $(TARGET)

# Правило для компиляции исполняемого файла
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Правило для очистки
clean:
	rm -f $(TARGET) *.o
