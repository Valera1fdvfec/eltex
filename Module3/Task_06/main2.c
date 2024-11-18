#include "chat.h"

int main() {
    int msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("Ошибка создания очереди сообщений");
        exit(1);
    }

    char buffer[MAX_TEXT];
    while (1) {
        receive_message(msgid, 1, buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Отправитель завершил чат.\n");
            cleanup_queue(msgid);
            break;
        }

        printf("Отправитель: %s\n", buffer);
        printf("Получатель: Введите сообщение: ");
        fgets(buffer, MAX_TEXT, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Убираем символ новой строки

        send_message(msgid, 2, buffer);
    }

    return 0;
}
