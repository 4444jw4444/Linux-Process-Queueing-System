.phony all:
all: PQS

PQS: PQS.c
	gcc PQS.c -lreadline -lhistory -ltermcap -pthread -O0 -g -o PQS

.PHONY clean:
clean:
	-rm -rf *.o *.exe