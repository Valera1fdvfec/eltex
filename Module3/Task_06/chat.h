#ifndef CHAT_H
#define CHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>

#define QUEUE_KEY 1234
#define MAX_TEXT 128

typedef struct {
    long msg_type; // Тип сообщения (приоритет)
    char text[MAX_TEXT]; // Текст сообщения
} Message;

// Функции для работы с очередью
void send_message(int msgid, long msg_type, const char *text);
void receive_message(int msgid, long msg_type, Message *message);
void cleanup_queue(int msgid);

#endif
