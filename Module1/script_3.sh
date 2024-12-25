read -p "Введите имя процесса для контроля:" process_name
read -p "Введите периодичность проверки в секундах:" check_interval

start_process() {
echo "Запуск процесса" $process_name
$process_name &
}

start_process

while true; do 
if ! pgrep -x "$process_name" > /dev/null; then
echo  "Процесс $process_name не запущен. Запускаем снова!"
start_process
fi
sleep "$check_interval"

done

