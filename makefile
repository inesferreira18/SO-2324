default: client orchestrator

client: bin/client.c bin/client.h
	gcc -o client bin/client.c

orchestrator: bin/orchestrator.c bin/orchestrator.h
	gcc -o orchestrator bin/orchestrator.c

clean:
	rm client orchestrator temp/*