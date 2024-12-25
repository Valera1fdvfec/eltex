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


start_time=$(echo "$start_time" | sed 's/T/ /')
end_time=$(echo "$end_time" | sed 's/T/ /')


start_epoch=$(date -d "$start_time" +%s)
end_epoch=$(date -d "$end_time" +%s)
grep -a -E "$process_name\[[0-9]+\]" "$log_file" | while IFS= read -r log; do

log_time_str=$(echo "$log" | awk '{print $1}' | sed 's/T/ /' | cut -d'.' -f1)
log_epoch=$(date -d "$log_time_str" +%s 2>/dev/null)


if [[ -n "$log_epoch" ]] && [[ $log_epoch -ge $start_epoch ]] && [[ $log_epoch -le $end_epoch ]]; then
echo "$log"
fi
done
}

# Читаем  файл
config_file="config.txt"

if [ ! -f "$config_file" ]; then
echo "Ошибка: файл конфигурации $config_file не найден."
exit 1
fi

# Считываем значения из конфигурационного файла
log_file="$(grep '^log_file=' "$config_file" | cut -d'=' -f2)"
process_name="$(grep '^process_name=' "$config_file" | cut -d'=' -f2)"
start_time="$(grep '^start_time=' "$config_file" | cut -d'=' -f2)"
end_time="$(grep '^end_time=' "$config_file" | cut -d'=' -f2)"

#Выводим путь
echo "Путь к файлу логов: $log_file"

parse_logs "$log_file" "$process_name" "$start_time" "$end_time"
