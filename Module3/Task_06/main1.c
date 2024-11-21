// программа отправителя
#include "chat.h"

int main() {
    int msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("Ошибка создания очереди сообщений");
        exit(1);
    }

    char buffer[MAX_TEXT];
    int priority;
    Message other_message;

    while (1) {
        // Ввод сообщения и приоритета
        printf("Введите сообщение (или 'exit' для выхода): ");
        fgets(buffer, MAX_TEXT, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Убираем символ новой строки

        if (strcmp(buffer, "exit") == 0) {
            send_message(msgid, 1, "exit");
            break;
        }

        do {
            printf("Введите приоритет сообщения (1 - высокий, 10 - низкий): ");
            if (scanf("%d", &priority) != 1 || priority < 1 || priority > 10) {
                printf("Ошибка: приоритет должен быть числом от 1 до 10. Попробуйте снова.\n");
                while (getchar() != '\n'); // Очистка ввода
                continue;
            }
            break;
        } while (1);
        while (getchar() != '\n'); // Очистка ввода

        // Отправляем сообщение получателю
        send_message(msgid, 1, buffer); // Тип 1 для сообщения получателю

        // Ожидание ответа от получателя
        printf("Ожидание ответа от собеседника...\n");
        receive_message(msgid, 2, &other_message); // Принимаем сообщение с типом 2

        if (strcmp(other_message.text, "exit") == 0) {
            printf("Собеседник завершил чат.\n");
            break;
        }

        printf("Ответ от собеседника: %s\n", other_message.text);
    }

    cleanup_queue(msgid);
    return 0;
}
