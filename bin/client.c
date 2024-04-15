#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "client.h"

void listenBack(int fd){
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
    int fdping = open(contoserver, O_WRONLY);
    if(fdping < 0)
    {
        printf("communication to server failed\n");
        return -1;
    }
    
    TASKS task;
    task.fd = getpid();

    if(strcmp("status", argv[1]) == 0){
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
        strcpy((&task.argument), argv[4]);
    }
    printf("%d\n%d\n%s\n", task.fd, task.time, task.argument);

    char answerpipe[64];
    sprintf(answerpipe, "temp/%d\0", getpid());
    mkfifo(answerpipe, 0777);





    write(fdping, &task, sizeof(TASKS));
    printf("%s\n", answerpipe);
    close(fdping);
    int ap = open(answerpipe, O_RDONLY);
    listenBack(ap);

}
