#!/bin/bash

create_backup() {
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
BACKUP_NAME="backup_$TIMESTAMP.tar.gz"
tar -czvf "$DEST_DIR/$BACKUP_NAME" "$SOURCE_DIR"
echo "[$(date)] Резервная копия $SOURCE_DIR успешно создана в $DEST_DIR/$BACKUP_NAME"
}

# Функция для установки задания в cron
schedule_backup() {

crontab -l | grep -v "$SCRIPT_PATH" | crontab -

case "$SCHEDULE" in
"hourly")
(crontab -l 2>/dev/null; echo "0 * * * * $SCRIPT_PATH") | crontab -
echo "Резервное копирование будет запускаться каждый час."
;;
"daily")
(crontab -l 2>/dev/null; echo "0 0 * * * $SCRIPT_PATH") | crontab -
echo "Резервное копирование будет запускаться каждый день в полночь."
;;
"weekly")
(crontab -l 2>/dev/null; echo "0 0 * * 0 $SCRIPT_PATH") | crontab -
echo "Резервное копирование будет запускаться каждое воскресенье в полночь."
;;
"monthly")
(crontab -l 2>/dev/null; echo "0 0 1 * * $SCRIPT_PATH") | crontab -
echo "Резервное копирование будет запускаться первого числа каждого месяца в полночь."
;;
*)
echo "Неправильный выбор периодичности. Задание не будет добавлено в cron."
exit 1
;;
esac
}

# Проверка параметров и запросы у пользователя
echo "Добро пожаловать в программу резервного копирования."

# Запрос исходной директории для копирования
read -p "Введите путь к директории, которую нужно сохранить: " SOURCE_DIR
if [ ! -d "$SOURCE_DIR" ]; then
echo "Ошибка: директория $SOURCE_DIR не найдена!"
exit 1
fi

# Запрос директории назначения для сохранения резервных копий
read -p "Введите путь к директории, куда будет сохранена резервная копия: " DEST_DIR
if [ ! -d "$DEST_DIR" ]; then
echo "Ошибка: директория $DEST_DIR не найдена!"
exit 1
fi


echo "Выберите периодичность резервного копирования:"
echo "1) Каждый час"
echo "2) Каждый день"
echo "3) Каждую неделю"
echo "4) Каждый месяц"

read -p "Введите номер выбора (1-4): " PERIOD

case $PERIOD in
1) SCHEDULE="hourly";;
2) SCHEDULE="daily";;
3) SCHEDULE="weekly";;
4) SCHEDULE="monthly";;
*) echo "Неправильный ввод. Выход."; exit 1;;
esac


SCRIPT_PATH=$(realpath "$0")

# Выполняем первое резервное копирование
create_backup

# Настраиваем cron для регулярного выполнения
schedule_backup

echo "Резервное копирование настроено и будет выполняться в соответствии с указанной периодичностью."
