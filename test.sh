rm logs/*
date +%T > test.txt
for run in {1..1000}; do
	s="$((run/1000)).$((run%100))"
	echo $LOGSNO
	./client execute $s -u "sleep $s"
	if [ $(ls logs | wc -l) = '800' ]; then
		date +%T >> test.txt
	fi
done