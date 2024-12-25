#!/bin/bash

for((i=1..3); do
mkdir "folder_$i"
for j in {1..3}; do
mkdir "folder_$i/$subfolder_$j" 
for k in {1..3}; do
touch "folder_$i/$subfolder_$j/$file_$k"
done
done
done

echo "Структура папок и файлов создана!"
