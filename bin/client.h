#ifndef _client_h
#define _client_h

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

void listenBack(int fd);

#endif
