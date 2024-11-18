#include "chat.h"

int main() {
    int msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("Ошибка создания очереди сообщений");
        exit(1);
    }

    char buffer[MAX_TEXT];
    while (1) {
        printf("Отправитель: Введите сообщение: ");
        fgets(buffer, MAX_TEXT, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Убираем символ новой строки

        send_message(msgid, 1, buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Завершаем отправку сообщений.\n");
            send_message(msgid, END_PRIORITY, "end");
            break;
        }

        receive_message(msgid, 2, buffer);
        printf("Получатель: %s\n", buffer);
    }

    return 0;
}
