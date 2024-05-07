#include "taskqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initQueue(QUEUE* queue){
    queue->first = NULL;
    queue->last = NULL;
}

int isQueueEmpty(QUEUE* queue){
    return (queue->first == NULL && queue->last == NULL);
}

void putInQueue(QUEUE* queue, TASKS* newTask){
    NODE* newNode = malloc(sizeof(struct node));

    if (newNode == NULL){
        //não foi possível criar um espaço na fila
        //mandar para o stderr(?)
        return;
    }

    //newNode->task = newTask;
    memcpy(&(newNode->task), newTask, sizeof(*newTask));
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
TASKS* pop2(QUEUE* queue) {
        if (isQueueEmpty(queue)) {
            //a fila está vazia, não há nada para tirar
            return NULL;
        }
    
        // Find the node with the least time
        NODE* auxNode = queue->first;
        NODE* lessTimeNode = queue->first;
        TASKS* lessTimeTask = &(auxNode->task);
    
        while (auxNode != NULL) {
            if (auxNode->task.time < lessTimeNode->task.time) {
                lessTimeNode = auxNode;
                lessTimeTask = &(auxNode->task);
            }
            auxNode = auxNode->next;
        }
    
        // If the node to be removed is the first node
        if (lessTimeNode == queue->first) {
            queue->first = queue->first->next;
        } 
        else {
            // Find the node before the one to be removed
            auxNode = queue->first;
            while (auxNode->next != lessTimeNode) {
                auxNode = auxNode->next;
            }
            // Adjust pointers to bypass the node to be removed
            auxNode->next = lessTimeNode->next;
        }
    
        // If the node to be removed is also the last node
        if (lessTimeNode == queue->last) {
            queue->last = auxNode; // Update the last pointer
        }
    
        // Free the memory occupied by the node
        free(lessTimeNode);
    
        return lessTimeTask;
    }

