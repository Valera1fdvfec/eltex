#!/bin/bash

read -p "Введите путь для создания папок (по умолчанию текущая директория): " base_path
base_path=${base_path:-.}

read -p "Введите количество папок (по умолчанию 3): " num_folders
num_folders=${num_folders:-3}

read -p "Введите шаблон имени (по умолчанию 'folder_#'): " name_template
name_template=${name_template:-"folder"}

read -p "Введите количество подпапок (по умолчанию 3): " num_subfolders
num_subfolders=${num_subfolders:-3}

read -p "Введите шаблон имени подпапки (по умолчанию 'subfolder_#'): " name_subtemplate
name_subtemplate=${name_subtemplate:-"subfolder"}

read -p "Введите количество файлов (по умолчанию 3): " num_file
num_file=${num_file:-3}

read -p "Введите шаблон имени файла (по умолчанию 'file_#'): " name_file
name_file=${name_file:-"file"}

for ((i=1; i<=num_folders; i++)); do
folder_name=${name_template//#/$i}
mkdir "$base_path/$folder_name"

for ((j=1; j<=num_subfolders; j++)); do
subfolder_name=${name_subtemplate//#/$j}
mkdir "$base_path/$folder_name/$subfolder_name"

for ((k=1; k<=num_file; k++)); do
file_name=${name_file//#/$k}
touch "$base_path/$folder_name/$subfolder_name/$file_name"

done
done
done

echo "Структура папок и файлов успешно создана в $base_path."
