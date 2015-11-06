.phony all:
all: PQS

PQS: PQS.c
	gcc PQS.c -lreadline -lhistory -ltermcap -pthread -o PQS

.PHONY clean:
clean:
	-rm -rf *.o *.exe