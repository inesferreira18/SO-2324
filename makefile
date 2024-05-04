default: client orchestrator

client: bin/client.c bin/client.h
	gcc -o client bin/client.c

orchestrator: bin/orchestrator.c bin/orchestrator.h parse.o
	gcc -o orchestrator bin/orchestrator.c parse.o taskqueue.o

parse.o: bin/parse.c bin/parse.h taskqueue.o
	gcc -c bin/parse.c

taskqueue.o: bin/taskqueue.c bin/taskqueue.h
	gcc -c bin/taskqueue.c

clean:
	rm client orchestrator parse.o taskqueue.o temp/*