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
int createFifo(char *path){
    if(access(path, F_OK) != 0){            //checks if pipe exists
        if(mkfifo(path, 0666) != 0)
        {
            printf("fifo not opened correctly\n");
            return -1;
        }
    }
    return(open(path, O_RDONLY | O_ASYNC));
}
int main(int argc, char **argv){

    umask(0);

    char *fifopath = "temp/contoserver";
    
    int fifo = createFifo(fifopath);
    
    TASKS newtask;

    int bytesread;
    while(1)
    {   
        if(bytesread = read(fifo, &newtask, sizeof(TASKS)) != 0){
           //printf("%d\n", bytesread);
           printf("pipe to answer: %d\ntime: %d\ntask: %s\n", newtask.fd, newtask.time, newtask.argument);
           writeback(newtask.fd);

           printf("fim do codigo do Longo :)\n");
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
            int logfd = open(filelog, O_WRONLY | O_CREAT);
            //char* file = strcat("logs/", itoa(newtask.fd));
            
            //int execvp(filelog,args);
    
        }

        
    }
}
