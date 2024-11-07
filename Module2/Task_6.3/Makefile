# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -g -fPIC

# Исходные файлы для динамических библиотек
ADD_SRC = add.c
SUBTRACT_SRC = subtract.c
MULTIPLY_SRC = multiply.c
DIVIDE_SRC = divide.c

# Динамические библиотеки
LIBS = libadd.so libsubtract.so libmultiply.so libdivide.so

# Имя исполняемого файла
TARGET = calculator

# Правило сборки всех библиотек и исполняемого файла
all: $(LIBS) $(TARGET)

# Правило для создания динамических библиотек
libadd.so: $(ADD_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $<

libsubtract.so: $(SUBTRACT_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $<

libmultiply.so: $(MULTIPLY_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $<

libdivide.so: $(DIVIDE_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $<


# Правило сборки исполняемого файла
$(TARGET): main.c $(LIBS)
	$(CC) -Wall -g main.c -ldl -o $(TARGET)

export LD_LIBRARY_PATH=.
# Правило очистки скомпилированных файлов
.PHONY: clean
clean:
	rm -f $(TARGET) $(LIBS)
