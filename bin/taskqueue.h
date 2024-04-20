#include "orchestrator.h"

typedef struct node{
    TASKS task;
    struct node* next;
}NODE;

typedef struct queue{
    NODE* first;
    NODE* last;
}QUEUE;

void initQueue(QUEUE *queue);
int isQueueEmpty(QUEUE *queue);
void putInQueue(QUEUE *queue, TASKS newTask);
TASKS* takeFromQueue(QUEUE *queue);
void clearQueue(QUEUE *queue);