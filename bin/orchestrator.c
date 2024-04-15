#include "orchestrator.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void writeback(int pid){
    char path[64];
    sprintf(path, "temp/%d\0", pid);
    int pipe = open(path, O_WRONLY | O_CREAT | O_ASYNC);
    write(pipe, "answer here", sizeof("answer here")+1);
    close(pipe);
}

int main(int argc, char **argv){

    umask(0);

    char *fifopath = "temp/contoserver";
    if(access(fifopath, F_OK) != 0){            //checks if pipe exists
        if(mkfifo(fifopath, 0666) != 0)
        {
            printf("fifo not opened correctly\n");
            return -1;
        }
    }
    printf("im here\n");
    int fifo = open(fifopath, O_RDONLY | O_ASYNC);
    printf("im here now tho\n");
    TASKS newtask;
    int bytesread;
    while(1)
    {   
        if(bytesread = read(fifo, &newtask, sizeof(TASKS)) != 0){
           //printf("%d\n", bytesread);
           printf("pipe to answer: %d\ntime: %d\ntask: %s\n", newtask.fd, newtask.time, newtask.argument);
           writeback(newtask.fd);
    
    
        }
    }
}
