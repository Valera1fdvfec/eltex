#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Использование: $0 <server_ip> <n>"
    echo "  <server_ip> - IP-адрес сервера (например, 192.168.1.100)"
    echo "  <n> - Количество клиентов для запуска"
    exit 1
fi

SERVER_IP="$1"  
NUM_CLIENTS="$2"  
BASE_PORT=50000

# Проверка, является ли NUM_CLIENTS положительным числом
if ! [[ "$NUM_CLIENTS" =~ ^[0-9]+$ ]] || [ "$NUM_CLIENTS" -le 0 ]; then
    echo "Ошибка: <n> должно быть положительным числом"
    exit 1
fi

# Проверка на файл client
if [ ! -f "./client" ]; then
    echo "Ошибка: Исполняемый файл client не найден"
    exit 1
fi
while true; do
	echo "Запуск нового цикла с $NUM_CLIENTS клиентами..."
	for ((i=0; i<NUM_CLIENTS; i++))
	do
		# порт для клиента
		CLIENT_PORT=$((BASE_PORT + i))

		# Генерируем случайное число
		RANDOM_NUMBER=$((RANDOM % 1000 + 1))

		# Запускаем клиент, передавая случайное число через stdin
		echo "$RANDOM_NUMBER" | ./client "$SERVER_IP" "$CLIENT_PORT" &

		CLIENT_PID=$!
		echo "Запущен клиент $((i+1)) с портом $CLIENT_PORT, PID: $CLIENT_PID, отправлено число: $RANDOM_NUMBER"

		sleep 0.5
	done

	wait

	echo "Цикл завершён, начинаем новый цикл..."
done
