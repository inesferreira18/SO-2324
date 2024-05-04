#include "taskqueue.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int countfunctions(TASKS newtask);
int contTitleEArgument(char* task);
char** storeFuctions (int count, TASKS newtask);
char** storeTitleAndArguments (int count, char* task);
char** storeTitleAndArguments (int count, char* task);
void free_function(char** argumentos);
char** parse(TASKS newtask);

