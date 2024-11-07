# Имя компилятора
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -g

# Файлы и цели
LIBRARY = libcontacts.a
OBJECTS = contact.o
TARGET = main

# Правило для компиляции основной программы (первая цель)
$(TARGET): main.c $(LIBRARY)
	$(CC) $(CFLAGS) main.c -L. -lcontacts -o $(TARGET)

# Правило для создания статической библиотеки
$(LIBRARY): $(OBJECTS)
	ar rcs $(LIBRARY) $(OBJECTS)

# Правило для создания объектного файла из contact.c
contact.o: contact.c contact.h
	$(CC) $(CFLAGS) -c contact.c

# Правило очистки сгенерированных файлов
clean:
	rm -f $(TARGET) $(OBJECTS) $(LIBRARY)
