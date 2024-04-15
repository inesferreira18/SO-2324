default: client orchestrator

client: client.c client.h
	gcc -o client bin/client.c

orchestrator: orchestrator.c orchestrator.h
	gcc -o orchestrator bin/orchestrator.c

clean:
	rm client orchestrator temp/*