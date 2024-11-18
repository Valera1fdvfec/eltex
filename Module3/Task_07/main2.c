#include "chat_posix.h"

int main() {
    struct mq_attr attr = {0};
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_TEXT;

    mqd_t mq1 = mq_open(QUEUE_NAME_1, O_CREAT | O_RDONLY, 0666, &attr);
    if (mq1 == -1) {
        perror("Ошибка создания очереди сообщений");
        exit(1);
    }

    mqd_t mq2 = mq_open(QUEUE_NAME_2, O_CREAT | O_WRONLY, 0666, &attr);
    if (mq2 == -1) {
        perror("Ошибка создания очереди сообщений");
        exit(1);
    }

    char buffer[MAX_TEXT];

    while (1) {
        printf("Отправитель: Введите сообщение: ");
        fgets(buffer, MAX_TEXT, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Убираем символ новой строки

        send_message(mq1, buffer, 1);

        if (strcmp(buffer, "exit") == 0) {
            printf("Завершаем отправку сообщений.\n");
            break;
        }

        unsigned int priority;
        receive_message(mq2, buffer, &priority);
        printf("Получатель: %s\n", buffer);
    }

    cleanup_queue(QUEUE_NAME_2);
    mq_close(mq1);
    mq_close(mq2);

    return 0;
}
