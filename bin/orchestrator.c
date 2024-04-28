#include "orchestrator.h"
#include "taskqueue.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

void writeback(int pid){ //pipe entre servidor -> cliente
    char path[64];
    sprintf(path, "temp/%d\0", pid);    	        //path é temp/[pid do cliente]
    int pipe = open(path, O_WRONLY | O_CREAT | O_ASYNC);  //o pipe é criado pelo cliente então pode-se só abrir
    write(pipe, "answer here", sizeof("answer here")+1); 
    close(pipe);
}
int createFifo(char *path){
    if(access(path, F_OK) != 0){            //checks if pipe exists
        if(mkfifo(path, 0666) != 0)         //creates pipe if it doesnt
        {
            printf("fifo not opened correctly\n");
            return -1;
        }
    }
    return(open(path, O_RDONLY | O_ASYNC));
}


//executa task (passar task.argument em lista de strings, id é o identificador da task)
//A SER USADO APENAS POR UM PROCESSO FILHO DEDICADO A ESTA TASK (pq paralelismo)
void execute_task(char *arg[], char *output_folder, int id){
    
    char end[100];                               //ficheiro destino
    sprintf(end, "%s/%d", *output_folder, id);

    int output = open(end, O_CREAT | O_WRONLY); //ficheiro destino aberto
    int saved_stdout = dup(1);
    dup2(output, 1);                            //execvp agora escreve para o ficheiro em vez de escrever para STD output
    clock_t starttime = clock(), endtime;     //starttime guardado
    execvp(arg[0], arg);                        //executado
    endtime = clock() - starttime;              //endtime guardado
    double time = (endtime * 1000) / CLOCKS_PER_SEC;        //time é os clock cycles a dividir pela frequencia do relogio (var global CLOCKS_PER_SEC do time.h)
    char answer[100];
    sprtinf(answer, "%d, '%s' time: %d", id, arg, time);
    write(output, answer, strlen(answer));
    write(output, itoa(time), sizeof(itoa(time)));
    close(output);
    dup2(saved_stdout, 1);                      //restaurar standard output (fanado do stack overflow)
    close(saved_stdout);
    _exit(0);
}


//It answer status. That's it, pretty self explanatory. answer_pipe is the answer pipe, queue is the queue, end_feito is where the ended things are feitas.
void answer_status(int answer_pipe, QUEUE queue, char *end_feito){
    char dir[strlen(end_feito)+2];
    sprintf(dir, "%s/*", *end_feito);
    write(answer_pipe, "ANSWERING:\n", sizeof("ANSWERING:\n"));
    NODE *walker;
    walker = queue.first;
    char* answer[100];
    write(answer_pipe, *answer, strlen(answer));
    while(walker != queue.last){           //não sabia se a lista dava loop no fim então fiz assim
        sprintf(answer, "%d: %s\n", walker->task.fd, walker->task.argument);
        write(answer_pipe, answer, strlen(answer));
        walker = walker->next;
    }

    write(answer_pipe, "\n\nDONE:\n", strlen("\n\nDONE:\n"));
    int saved_output = dup(1);
    dup2(answer_pipe, 1);
    execlp("tail", "tail", "-n", "1", dir, NULL);   //bash para "dá-me a ultima linha destas coisas todas", que "convenientemente" está no formato perfeito *wink wink*
    dup2(saved_output, 1);
    close(saved_output);
}



int main(int argc, char **argv){
    QUEUE queue;        
    initQueue(&queue);

    umask(0);

    char *fifopath = "temp/contoserver"; //pipe entre cliente -> servidor
    
    int fifo = createFifo(fifopath);
    
    TASKS newtask;                      //Task tem fd, time, argument e type

    int bytesread;
    while(1){   
        if(bytesread = read(fifo, &newtask, sizeof(TASKS)) != 0){       //certificar que ele lê alguma coisa (para os primeiros reads onde não há ninguem a escrever e devolve -1)
            //printf("%d\n", bytesread);                                //otherwise ele em teoria congela no read
            printf("pipe to answer: %d\ntime: %d\ntask: %s\n", newtask.fd, newtask.time, newtask.argument);
            writeback(newtask.fd);                                      //função básica para responder ao cliente depois de receber task

            putInQueue(&queue, newtask); //adiciona a nova tarefa à fila

            if(!isQueueEmpty(&queue)){ // se houver uma próxima tarefa na fila
                TASKS newTask = queue.first->task;

                //mandar para o ficheiro "em execução" e tirar do ficheiro "em espera"

                //faltam cenas aqui ------------------------------------------------------------------------------------------
            }

            int count = 1;
            for(int j = 0; j<sizeof(newtask.argument);j++){
                if(newtask.argument[j]==' '){
                    count++;
                    printf("%d",count);
                }
            }
            char** args = malloc(count+1);
            char* argumento;
            argumento = strtok(newtask.argument," ");
            int i = 0;
            while (argumento!=NULL)
            {
                args[i] = malloc(sizeof(*argumento));
                printf("%s",*argumento);
                strcpy(args[i],*argumento);
                argumento = strtok(NULL," ");
                i++;
            }

            char filelog[64];
            sprintf(filelog, "logs/%d\0",newtask.fd);
            int logfd = open(filelog, O_WRONLY | O_CREAT); //criação do ficheiro que armazena todas as tarefas
            //char* file = strcat("logs/", itoa(newtask.fd));
            
            //int execvp(filelog,args);
    
        }
        
        
    }
}
