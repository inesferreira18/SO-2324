default: client orchestrator

client: bin/client.c bin/client.h
	gcc -o client bin/client.c bin/parse.c

orchestrator: bin/orchestrator.c bin/orchestrator.h bin/parse.c bin/parse.h bin/taskqueue.c bin/taskqueue.h
	gcc -o orchestrator bin/orchestrator.c bin/parse.c bin/taskqueue.c

clean:
	rm client orchestrator parse.o taskqueue.o temp/* pipes/*

cleanlogs:
	rm logs/*