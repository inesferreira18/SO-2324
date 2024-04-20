#include "taskqueue.h"
#include <stdio.h>

void initQueue(QUEUE* queue){
    queue->first = NULL;
    queue->last = NULL;
}

int isQueueEmpty(QUEUE* queue){
    return (queue->first == NULL && queue->last == NULL);
}

void putInQueue(QUEUE* queue, TASKS newTask){
    NODE* newNode = malloc(sizeof(struct node));

    if (newNode == NULL){
        //não foi possível criar um espaço na fila
        //mandar para o stderr(?)
        return;
    }

    newNode->task = newTask;
    newNode->next = NULL; 

    if (isQueueEmpty(queue)){
        queue->first = newNode;
        queue->last = newNode;
    }
    else{ // se não estiver vazia, mete no fundo da fila
        queue->last->next = newNode;
        queue->last = newNode;
    }
}

//retorna um apontador para a estrutura porque assim podemos aceder ao que saiu, posteriormente
TASKS* takeFromQueue (QUEUE* queue){
    if(isQueueEmpty(queue)){
        //a fila está vazia, não há nada para tirar
        return NULL;
    }

    NODE* auxNode = queue->first;
    TASKS* auxTask = &(auxNode->task);

    if(queue->first == queue->last){ //tem só um elemento
        queue->first = NULL;
        queue->last = NULL;
    }
    else{ //sai o primeiro, e o seguinte passa ao primeiro. o último continua o mesmo
        queue->first = queue->first->next;
    }

    free(auxNode);
    return auxTask;
}

void clearQueue (QUEUE* queue){
    while(!isQueueEmpty(queue)){
        takeFromQueue(queue);
    }
}