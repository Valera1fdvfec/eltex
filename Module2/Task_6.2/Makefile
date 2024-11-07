# Имя компилятора
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -g -fPIC

# Файлы и цели
LIBRARY = libcontacts.so
OBJECTS = contact.o
TARGET = main

# Правило для компиляции основной программы с подключением библиотеки
$(TARGET): main.c $(LIBRARY)
	$(CC) $(CFLAGS) main.c -L. -lcontacts -Wl,-rpath=. -o $(TARGET)

# Правило для создания динамической библиотеки
$(LIBRARY): $(OBJECTS)
	$(CC) -shared -o $(LIBRARY) $(OBJECTS)

# Правило для создания объектного файла из contact.c
contact.o: contact.c contact.h
	$(CC) $(CFLAGS) -c contact.c

# Установка переменной среды для поиска библиотеки и запуск программы
all: $(TARGET)
	export LD_LIBRARY_PATH=.; ./$(TARGET)

# Правило очистки сгенерированных файлов
clean:
	rm -f $(TARGET) $(OBJECTS) $(LIBRARY)
