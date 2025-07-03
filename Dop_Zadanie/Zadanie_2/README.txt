Задание 2: fork + poll/epoll/select + timers + IPC

Управление такси.
Есть опорный пункт - исходное приложение. Запускается один раз и привязано к терминалу, имеет CLI.

В CLI должны быть команды
 * create_driver
 * send_task <pid> <task_timer>
 * get_status <pid>
 * get_drivers

create_driver создает новый процесс (driver), который ожидает команд от CLI, pid процесса будет использоваться  для обращения к этому driver.

send_task <pid> <task_timer> создаст задачу для driver номер <pid> и займет его на <task_timer> секунд. Если попытаться обратиться к этому driver во время выполнения задания с помощью send_task, то driver должен послать ошибку Busy <task_timer>. По истечению <task_timer> секунд driver меняет свое состояние на Available.

get_status <pid> показывает статус driver с номером <pid>. Может быть Busy <task_timer> либо Available.

get_drivers показывает статусы и pid всех drivers запущенных из этого инстанса CLI.

Описание добавленных файлов:

	сommon.h: Заголовочный файл, содержащий общие определения: константы MAX_DRIVERS (максимум 100 драйверов) и MSG_SIZE (размер сообщения 128 байт), перечисление DriverStatus (AVAILABLE, BUSY) и структуру DriverInfo для хранения PID, статуса, дескрипторов канала и времени окончания задачи.
	
	Makefile: Скрипт сборки проекта. Компилирует два исполняемых файла: cli (из cli_tax.c) и driver (из driver.c) с флагом -Wall. Содержит цель clean для удаления скомпилированных файлов.
	
	main.c: CLI-интерфейс. Принимает команды: create_driver (создает драйвер), send_task <pid> <time> (отправляет задачу), get_status <pid> (проверяет статус), get_drivers (список драйверов).
	driver.c: Процесс-драйвер. Читает задачи ("TASK ") с канала (fd 3), выполняет их, уменьшая busy_time каждую секунду.
	send_task.c: Функция send_task(pid, task_time) отправляет задачу драйверу по PID через канал, устанавливает статус BUSY и время окончания.
	get_status.c: Функции для поиска драйвера (find_driver), обновления статуса (update_status) и вывода статуса (get_status): "Busy " или "Available".
	create_drivers.c: Создает драйверы через fork, настраивает канал, запускает driver через execl, сохраняет данные в массив drivers.
	get_drivers.c: Выводит список всех драйверов с их PID и статусами ("Busy " или "Available").
