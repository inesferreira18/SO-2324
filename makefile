default: client orchestrator

client: client.c client.h
	gcc -o client client.c

orchestrator: orchestrator.c orchestrator.h
	gcc -o orchestrator orchestrator.c

clean:
	rm client orchestrator temp/contoserver