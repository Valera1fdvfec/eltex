#ifndef CHAT_POSIX_H
#define CHAT_POSIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>

#define QUEUE_NAME_1 "/chat_queue_1"
#define QUEUE_NAME_2 "/chat_queue_2"
#define MAX_TEXT 128
#define END_PRIORITY 10

typedef struct {
    char text[MAX_TEXT];
} Message;

// Функции для работы с очередью
void send_message(mqd_t mq, const char *text, unsigned int priority);
void receive_message(mqd_t mq, char *buffer, unsigned int *priority);
void cleanup_queue(const char *queue_name);

#endif
