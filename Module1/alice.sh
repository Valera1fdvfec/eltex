#!/bin/bash

# Убедитесь, что файл с книгой на русском загружен
book_file="alice.txt"

# Проверяем, существует ли файл с книгой
if [ ! -f "$book_file" ]; then
    echo "Ошибка: файл книги не найден!"
    exit 1
fi
letters_count=$( grep -o "[А-Яа-яA-Za-z]" "$book_file" | wc -l)
echo "Количество букв в книге: $letters_count"

#Считаем количествo слов в книге
words_count=$(wc -w < "$book_file")
echo "Количество слов в книге: $words_count"

#Ищем первое предложение с упоминанием королевы
first_queen_sentence=$(grep -o '[^.!?]*[Кк]оролев[аеуы][^.!?]*[.!?]' "$book_file" | head -n 1)
if [ -z "$first_queen_sentence" ]; then
    echo "Упоминания королевы не найдено."
else
first_queen_sentence=$(echo "$first_queen_sentence" | sed 's/^ *//;s/ *$//')
    echo "Первое предложение с упоминанием королевы: $first_queen_sentence"
fi

