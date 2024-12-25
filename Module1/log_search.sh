#!/bin/bash

parse_logs() {
log_file="$1"
process_name="$2"
start_time="$3"
end_time="$4"

if [ ! -f "$log_file" ]; then
echo "Ошибка: файл $log_file не найден или это не файл."
exit 1
fi

start_epoch=$(echo "$start_time" | sed 's/T/ /')
end_epoch=$(echo "$end_time" | sed 's/T/ /')

start_epoch=$(date -d "$start_time" +%s)
end_epoch=$(date -d "$end_time" +%s)

grep -a -E  "$process_name\\[[0-9]+\\]" "$log_file" | while IFS= read -r log; do
log_time_str=$(echo "$log" | awk '{print $1}' | sed 's/T/ /' | cut -d '.' -f1)
log_epoch=$(date -d "$log_time_str" +%s 2>/dev/null)

if [[ -n "$log_epoch" ]] && [[ $log_epoch -ge $start_epoch ]] && [[ $log_epoch -le $end_epoch ]]; then

echo "$log"
fi

done
}

read -p "Введите путь к файлу логов: " log_file
read -p "Введите имя процесса: " process_name
read -p "Введите начальное время (формат: YYYY-MM-DDTHH:MM:SS): " start_time
read -p "Введите конечное время (формат: YYYY-MM-DDTHH:MM:SS): " end_time



parse_logs "$log_file" "$process_name" "$start_time" "$end_time"
