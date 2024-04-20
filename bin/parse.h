#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef enum requestType{
    simple,
    pipelined,
    status
}REQTYPE;

typedef struct tasks{
    int fd;
    int time;
    REQTYPE type;
    char argument[300];
}TASKS;

int countfunctions(TASKS newtask);
int contTitleEArgument(char* task);
char** storeFuctions (int count, TASKS newtask);
char** storeTitleAndArguments (int count, char* task);
char** storeTitleAndArguments (int count, char* task);
void parse(TASKS newtask);

