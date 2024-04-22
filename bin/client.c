#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "client.h"

void listenBack(int fd){            //função básica para o cliente ficar a ouvir a resposta do servidor
    char answer[64];
    int bytesread = read(fd, answer, 64);
    printf("%s\n", answer);
}

int main(int argc, char **argv){

    umask(0);

    //Check if arguments are correct
    if(argc != 1 && strcmp(argv[1], "status") == 0){
        printf("arguments invalid");
        return -2;
    }

    else if(argc != 5 || strcmp(argv[1], "execute") != 0 || argv[3][0] != '-'){
        printf("execute arguments not valid\n");
        //printf("argc = %d\ncommand = %s\nargv[3] = %c\n", argc, argv[1], argv[3][0]);
        return -2;
    }



    char *contoserver = "temp/contoserver";
    //mkfifo(contoserver, O_WRONLY | O_CREAT);
    int fdping = open(contoserver, O_WRONLY);       //abre pipe de escrita para o servidor
    if(fdping < 0)
    {
        printf("communication to server failed\n");
        return -1;
    }
    
    TASKS task;                                     //Task
    task.fd = getpid();                             //id unico é o pid do cliente

    if(strcmp("status", argv[1]) == 0){             //status tem um tipo de task especifico
        task.type = status;                         //time = -1 para prioridade máxima
        task.time = -1;
    }

    else{
        task.time = atoi(argv[2]);
        switch(argv[3][1]){
            case 'u':
                task.type = simple;                 //o argv já divide um monte de coisas então eu copio e formato o que já estiver parsed
                break;
        
            case 'p':
                task.type = pipelined;
                break;

            default:
                printf("type of task (%c) unknown\n", argv[3][1]);
                return -3;
      }
        strcpy((&task.argument), argv[4]);          //o argumento tem de ser parsed no servidor
    }
    //printf("%d\n%d\n%s\n", task.fd, task.time, task.argument);

    char answerpipe[64];
    sprintf(answerpipe, "temp/%d\0", getpid());
    mkfifo(answerpipe, 0777);                       //criação do fifo pelo cliente


                                                    //IMPORTANTE: o fifo tem de ser criado antes do servidor o tentar abrir mas o cliente congela se o abrir até alguém abrir o lado de escrita
                                                    //isto é por default no C, just fair warning (tive um dia inteiro a resolver e era só isto)

    write(fdping, &task, sizeof(TASKS));            //escrever para o servidor
    close(fdping);                                  //fechar o pipe (less clutter)
    int ap = open(answerpipe, O_RDONLY);            
    listenBack(ap);
    return 0;
}
