default: client orchestrator

client: src/client.c src/client.h
	gcc -o client src/client.c src/parse.c

orchestrator: src/orchestrator.c src/orchestrator.h src/parse.c src/parse.h src/taskqueue.c src/taskqueue.h
	gcc -o orchestrator src/orchestrator.c src/parse.c src/taskqueue.c

clean:
	rm client orchestrator parse.o taskqueue.o temp/* pipes/*

cleanlogs:
	rm logs/*