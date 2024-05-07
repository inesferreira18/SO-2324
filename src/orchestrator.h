void writeback(int pid);
int createFifo(char *path);
void execute_task(char *arg[], char *output_folder, int id);
void execute_singular(char *arg[], char *output_folder, int id);
void execute_pipeline(char *arg[], char *output_folder, int id);
void answer_status(int answer_pipe, QUEUE queue, char *end_feito);
void doasIsay(int childno, char *logpath, int notifyServ, int getTask);