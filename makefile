default: client orchestrator

client: client.c client.h
	gcc -c client client.c

orchestrator: orchestrator.c orchestrator.h
	gcc -c orchestrator orchestrator.c

rm:
	rm client orchestrator