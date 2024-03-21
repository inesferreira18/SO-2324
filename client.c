#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "client.h"

int main(int argc, char **argv){

    //Check if arguments are correct
    if(argc != 1 && strcmp(argv[1], "status")){
        printf("arguments invalid");
        return -2;
    }

    else if(argc != 4 || !strcmp(argv[1], "execute") || argv[3][0] != '-'){
        printf("execute arguments not valid\n");
        return -2;
    }



    char *contoserver = "tmp/contoserver";
    int fdping = open(contoserver, O_WRONLY);
    if(fdping == -1)
    {
        printf("communication to server failed\n");
        return -1;
    }

    int fd[2];
    if(pipe(fd) == -1)
        return -1;
    
    TASKS task;
    task.fd = fd[1];

    if(strcmp("status", argv[1])){
        task.type = status;
        task.time = -1;
    }

    else{
        task.time = atoi(argv[2]);
        switch(argv[3][1]){
            case 'u':
                task.type = simple;
                break;
        
            case 'p':
                task.type = pipelined;
                break;

            default:
                printf("type of task (%c) unknown\n", argv[3][1]);
                return -3;
      }
        strcpy(&task.argument, &argv[4]);
    }
    
    write(fdping, &task, sizeof(TASKS));
    listenBack(fd[0]);

}
