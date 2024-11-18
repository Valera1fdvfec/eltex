#include "chat.h"

void send_message(int msgid, long msg_type, const char *text) {
    Message message;
    message.msg_type = msg_type;
    strncpy(message.text, text, MAX_TEXT - 1);
    message.text[MAX_TEXT - 1] = '\0';

    if (msgsnd(msgid, &message, sizeof(message.text), 0) == -1) {
        perror("Ошибка отправки сообщения");
        exit(1);
    }
}

void receive_message(int msgid, long msg_type, char *buffer) {
    Message message;

    if (msgrcv(msgid, &message, sizeof(message.text), msg_type, 0) == -1) {
        perror("Ошибка получения сообщения");
        exit(1);
    }
    strncpy(buffer, message.text, MAX_TEXT - 1);
    buffer[MAX_TEXT - 1] = '\0';
}

void cleanup_queue(int msgid) {
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("Ошибка удаления очереди сообщений");
        exit(1);
    }
}
