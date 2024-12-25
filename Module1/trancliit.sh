#!/bin/bash

transliterate() {
echo "$1" | sed '
y/ABEKMHOPRCTYXGDLFZQJWabekmhoprctyxgdlfzqjw/АВЕКМНОПРСТУХГДЛФЗКЙВавекмнопрстухгдлфзкйв/;
s/X/Кс/g; s/x/кс/g
'
}

# Чтение строки от пользователя
read -p "Введите строку для транслитерации: " input

# Выполнение транслитерации и вывод результата
result=$(transliterate "$input")
echo "Результат транслитерации: $result"
