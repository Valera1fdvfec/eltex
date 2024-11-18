#include "chat_posix.h"

void send_message(mqd_t mq, const char *text, unsigned int priority) {
    if (mq_send(mq, text, strlen(text) + 1, priority) == -1) {
        perror("Ошибка отправки сообщения");
        exit(1);
    }
}

void receive_message(mqd_t mq, char *buffer, unsigned int *priority) {
    if (mq_receive(mq, buffer, MAX_TEXT, priority) == -1) {
        perror("Ошибка получения сообщения");
        exit(1);
    }
}

void cleanup_queue(const char *queue_name) {
    if (mq_unlink(queue_name) == -1) {
        perror("Ошибка удаления очереди сообщений");
    }
}
