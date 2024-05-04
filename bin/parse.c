#include "parse.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int countfunctions(TASKS newtask){
    int count = 1;
    int n = strlen(newtask.argument);
    for(int j = 0; j<n;j++){
        if(newtask.argument[j]=='|'){
            count++;
        }
    }
    printf("here\n");
    return count;
}

int contTitleEArgument(char* task){
    printf("%s\n", task);
    int count = 1;
    int n = strlen(task);
    for(int j = 0; j<n;j++){
        if(task[j]==' '){
            count++;
        }
    }
    printf("here\n");
    return count;
}

char** storeFuctions (int count, TASKS newtask){
    char** args = malloc(count);
    char* argumento;
    argumento = strtok(newtask.argument,"|");
    int i = 0;
    while (argumento != NULL)
    {
        args[i] = malloc(sizeof(argumento));
        strcpy(args[i],argumento);
        argumento = strtok(NULL,"|");
        printf("%s",args[i]);
        getchar();
        i++;
    }
    printf("AAAAAAAAAAAAAAAA");
    getchar();
    return args;
}

char** storeTitleAndArguments (int count, char* task){
    //acrescentar 1 ao count por causa do NULL no final
    char** args = malloc((count+1) * sizeof(int));
    char* argumento;
    argumento = strtok(task," ");
    int i = 0;
    while(argumento != NULL)
    {
        args[i] = malloc(strlen(argumento));
        strcpy(args[i],argumento);
        argumento = strtok(NULL," ");
        printf("%s",args[i]);
        getchar();
        printf("\n");
        i++;
    }
    args[i] = NULL;
    printf("%s",args[i]);
    getchar();
    printf("\n");
    return args;
}

void free_function(char** argumentos){
    int i=0;
    while(argumentos[i]!=NULL){
        free(argumentos[i]);
        i++;
    }
    free(argumentos[i]);
    free(argumentos);
}

char** parse(TASKS newtask){
    
    int count = contTitleEArgument(newtask.argument);
    char **args = storeTitleAndArguments(count,newtask.argument);
    return args;
        //char filelog[64];
        //sprintf(filelog, "logs/%d\0",newtask.fd);
        //int logfd = open(filelog, O_WRONLY | O_CREAT);
        //char* file = strcat("logs/", itoa(newtask.fd));
        //FALTA DUPS
        //int execvp(filelog,args);
}