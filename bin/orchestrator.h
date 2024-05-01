typedef enum requestType{
    simple,
    pipelined,
    status
}REQTYPE;

typedef enum queuePolicy{
    SJF,
    FCFS
}SCHEDPOL;

typedef struct tasks{
    int fd;
    int time;
    REQTYPE type;
    char argument[300];
}TASKS;




void execute_pipeline(char **arg, char* output_folder, int id);
void doasIsay(int childno, int notifyServ, int getTask);