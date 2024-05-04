typedef enum requestType{
    simple,
    pipelined,
    status
}REQTYPE;

typedef enum queuePolicy{
    SJF,
    FCFS
}SCHEDPOL;

typedef struct task{
    int fd;
    int time;
    REQTYPE type;
    char argument[300];
}TASKS;


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
void putInQueue(QUEUE *queue, TASKS* newTask);
TASKS* takeFromQueue(QUEUE *queue);
void clearQueue(QUEUE *queue);