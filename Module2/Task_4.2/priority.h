#ifndef PRIORITY_H
#define PRIORITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DATA_LENGTH 100
#define AUTO_GENERATED_LENGTH 10

// Определение структуры узла
typedef struct Node {
    int priority;
    char data[MAX_DATA_LENGTH];
    struct Node* next;
} Node;

// Определение структуры очереди с приоритетом
typedef struct PriorityQueue {
    Node* head;
} PriorityQueue;

// Объявление функций
PriorityQueue* createQueue();
Node* createNode(char* data, int priority);
void enqueue(PriorityQueue* pq, char* data, int priority);
void dequeue(PriorityQueue* pq);
void dequeueWithPriority(PriorityQueue* pq, int priority);
void dequeueWithMinPriority(PriorityQueue* pq, int minPriority);
void printQueue(PriorityQueue* pq);
void printAllWithMinPriority(PriorityQueue* pq, int minPriority);
void deleteQueue(PriorityQueue* pq);
void autoGenerateData(PriorityQueue* pq, int count);
void deleteNodeWithPriority(PriorityQueue* pq, int priority);

#endif // PRIORITY_H
