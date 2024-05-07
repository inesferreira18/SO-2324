#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "parse.h"
#include <stdlib.h>
#include <math.h>
#include <dirent.h>

void writeback(int pid){ //pipe entre servidor -> cliente
    char path[64];
    sprintf(path, "temp/%d\0", pid);    	        //path é temp/[pid do cliente]
    int pipe = open(path, O_WRONLY | O_ASYNC);  //o pipe é criado pelo cliente então pode-se só abrir
    char *answer = malloc(64);
    strcpy(answer, "answer here\n");
    write(pipe, answer, 12); 
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
//não mata o processo
void execute_task(char *arg[], char *output_folder, int id){
    for(int i = 0; arg[i] != NULL; i++){
        printf("%s\n", arg[i]);
    }
    char end[100];                               //ficheiro destino
    sprintf(end, "%s/%d", output_folder, id);
    creat(end, 0666);
    int output = open(end, O_CREAT | O_WRONLY); //ficheiro destino aberto
    int saved_stdout = dup(1);
    dup2(output, 1);                            //execvp agora escreve para o ficheiro em vez de escrever para STD output
    struct timespec starttime, endtime;                 //starttime guardado
    clock_gettime(CLOCK_REALTIME, &starttime);
    int pid = fork();
    if(pid == 0){  
        execvp(arg[0], arg);                        //executado
        _exit(1);
    }
    wait(NULL);
    clock_gettime(CLOCK_REALTIME, &endtime);
    double delta = (endtime.tv_sec - starttime.tv_sec) + (endtime.tv_nsec - starttime.tv_nsec) / 1e9;
    char answer[100];
    sprintf(answer, "\n%d time: %.4f\n", id, delta);
    write(output, answer, strlen(answer));

    close(output);
    dup2(saved_stdout, 1);                      //restaurar standard output (fanado do stack overflow)
    close(saved_stdout);
}

void execute_singular(char *arg[], char *output_folder, int id)
{
    char end[100];                               //ficheiro destino
    sprintf(end, "%s/%d", output_folder, id);
    creat(end, 0640);
    int output = open(end, O_CREAT | O_WRONLY); //ficheiro destino aberto
    int saved_stdout = dup(1);
    dup2(output, 1);                            //execvp agora escreve para o ficheiro em vez de escrever para STD output
    int pid = fork();
    if(pid == 0){  
        execvp(arg[0], arg);                        //executado
        _exit(1);
    }
    wait(NULL);
    close(output);
    dup2(saved_stdout, 1);                      //restaurar standard output (fanado do stack overflow)
    close(saved_stdout);
}

void execute_pipeline(char* arg[], char* output_folder, int id){

    char outputfile[50], tempfile[50], *tempstring, *tempstring2;
    sprintf(outputfile, "%s/%d", output_folder, id);
    sprintf(tempfile, "pipes/%d", id);
    int walker = 0, sectionstart = 0;
    struct timespec starttime, endtime;
    clock_gettime(CLOCK_REALTIME, &starttime);
    for(walker = 0; arg[walker] != NULL; walker++){
;
        if(strcmp(arg[walker], "|") == 0){

            if(sectionstart == 0){    
             
                tempstring = arg[walker];
                arg[walker] = NULL;
                execute_singular(&arg[sectionstart], "pipes", id);
                arg[walker] = tempstring;

                sectionstart = walker+1;
            }

            else{
                tempstring = arg[walker];
                tempstring2 = arg[walker+1];
                arg[walker] = tempfile;
                arg[walker+1] = NULL;
                execute_singular(&arg[sectionstart], "pipes", id);
                arg[walker] = tempstring;
                arg[walker+1] = tempstring2;
                sectionstart = walker+1;
            }

        }
    }
    arg = realloc(arg, (walker+1) * (sizeof(char*)));
    arg[walker] = tempfile;
    arg[walker+1] = NULL;
    if(fork()==0)
        execute_singular(&arg[sectionstart], output_folder, id); //só para forçar a não encavalitar o tempo com o fim
    //if(fork() == 0)
        //execlp("rm", "rm", tempfile, NULL);
    wait(NULL);
    clock_gettime(CLOCK_REALTIME, &endtime);
    double delta = (endtime.tv_sec - starttime.tv_sec) + (endtime.tv_nsec - starttime.tv_nsec) / 1e9;
    char answer[100];
    sprintf(answer, "\n%d time: %.4f\n", id, delta);
    int fd = open(outputfile, O_WRONLY | O_APPEND);
    write(fd, answer, strlen(answer));
    close(fd);

    arg = realloc(arg, sizeof(arg) - sizeof(char*));
    return;



}


//It answer status. That's it, pretty self explanatory. answer_pipe is the answer pipe, queue is the queue, end_feito is where the ended things are feitas.
void answer_status(int answer_pipe, QUEUE queue, char *end_feito){
    printf("answering now\n");
    char dir[strlen(end_feito)+10];
    sprintf(dir, "%s/*", end_feito);
    printf("dir is %s\n", dir);
    write(answer_pipe, "ANSWERING:\n", sizeof("ANSWERING:\n"));
    NODE *walker;
    walker = queue.first;
    char answer[100];
    while(walker != queue.last){           //não sabia se a lista dava loop no fim então fiz assim
        sprintf(answer, "%d: %s\n", walker->task.fd, walker->task.argument);
        write(answer_pipe, answer, strlen(answer));
        walker = walker->next;
    }

    write(answer_pipe, "\n\nDONE:\n", strlen("\n\nDONE:\n"));
    int saved_output = dup(1);
    dup2(answer_pipe, 1);
    //execlp("tail", "tail", "-n", "1", dir, NULL);   //bash para "dá-me a ultima linha destas coisas todas", que "convenientemente" está no formato perfeito *wink wink*
    DIR *d;
    struct dirent *directories;
    d = opendir(end_feito);
    if(d){
        while((directories = readdir(d)) != NULL)
        {
            if(directories->d_name[0] != '.'){
                    
                sprintf(dir, "%s/%s", end_feito, directories->d_name);
                if(fork() == 0)
                    execlp("tail", "tail", "-n", "1", dir, NULL);
                wait(NULL);
            }
        }
    }
    write(answer_pipe, "end_token", 10);
    dup2(saved_output, 1); //1 = answer_pipe aqui
    close(saved_output);
    close(answer_pipe);
}

void doasIsay(int childno, char* logpath,int notifyServ, int getTask){
    int i = -1;
    TASKS oldtask;
    oldtask.fd = 1;
    while(1){
        TASKS task;
        while(i <= 0)
            i = write(notifyServ, &childno, 4);
        i = -1;
        while(read(getTask, &task, sizeof(TASKS)) < 1){}
        char **args;
        if(oldtask.fd > 0 && oldtask.fd != task.fd){
        switch(task.type){
            case simple:
                args = parse(task);
                
                execute_task(args, logpath, task.fd);
                free_function(args);
                break;
            
            case pipelined:
                args = parse(task);
                execute_pipeline(args, logpath, task.fd);
                free_function(args);
                break;

            case status:
                QUEUE queue;
                read(getTask, &queue, sizeof(QUEUE));
                char* path = malloc(64);
                sprintf(path, "temp/%d\0", task.fd);    	        //path é temp/[pid do cliente]
                int pipe = open(path, O_WRONLY | O_CREAT | O_ASYNC);  //o pipe é criado pelo cliente então pode-se só abrir
                answer_status(pipe, queue, logpath);
                close(pipe);
                free(path);
                break;
            
            default:
                perror("fecked task\n");
                break;

        }
            oldtask = task;
        }
    }
}

int main(int argc, char **argv){
    
    //valores de inicialização
    umask(0);
    char *logpath = argv[1];
    int maxtask = atoi(argv[2]);
    SCHEDPOL policy;
    if(strcmp(argv[3], "SJF") == 0)
        policy = SJF;
    else if(strcmp(argv[3], "FCFS") == 0)
        policy = FCFS;
    else{
        perror("invalid policy\n");
        return -1;
    }
    
    //filhos escravos que fazem os pedidos
    //pipe[0] é pipe dos filhos a dizer quando estão disponiveis
    //cada outro pipe é do server a dizer ao filho respetivo que task fazer 
    int pipes[maxtask+1][2];
    pipe(pipes[0]);
    int childNo, babayaga;

    for(childNo = 1; childNo <= maxtask; childNo++){
        babayaga = pipe(pipes[childNo]);
        int pid = fork();
        if(pid == 0)
            doasIsay(childNo, logpath, pipes[0][1], pipes[childNo][0]);
    }
    
    QUEUE queue;        
    initQueue(&queue);


    char *fifopath = "temp/contoserver"; //pipe entre cliente -> servidor
    
    int fifo = createFifo(fifopath);
    
    TASKS newtask;                      //Task tem fd, time, argument e type

    int bytesread;
    int allTasksInQueue = 0; // flag para saber se já estão todas as tasks na queue, inicialmente não tem nenhuma 

    while(1){   
        if(bytesread = read(fifo, &newtask, sizeof(TASKS)) != 0){       //certificar que ele lê alguma coisa (para os primeiros reads onde não há ninguem a escrever e devolve -1)
            //printf("%d\n", bytesread);                                //otherwise ele em teoria congela no read
            printf("pipe to answer: %d\ntime: %d\ntask: %s\ntask type: %d\n", newtask.fd, newtask.time, newtask.argument, newtask.type);
            writeback(newtask.fd);                                      //função básica para responder ao cliente depois de receber task

            putInQueue(&queue, &newtask); //adiciona a nova tarefa à fila
            if (queue.last->next == NULL){ //quando a queue estiver cheia
                allTasksInQueue = 1;
            }

            
            //int execvp(filelog,args);
    
        } else if (!isQueueEmpty(&queue)){ // se houver uma próxima tarefa na fila
            TASKS newTask;
            newTask.fd = queue.first->task.fd;
            newTask.time = queue.first->task.time;
            newTask.type = queue.first->task.type;
            strcpy(newTask.argument, queue.first->task.argument);
            takeFromQueue(&queue);


            // mandar para o ficheiro "em execução" e tirar do ficheiro "em espera"
            read(pipes[0][0], &childNo, sizeof(int));
            write(pipes[childNo][1], &newtask, sizeof(TASKS));
            if(newtask.type == status)
                write(pipes[childNo][1], &queue, sizeof(QUEUE));
        }
    }
}
