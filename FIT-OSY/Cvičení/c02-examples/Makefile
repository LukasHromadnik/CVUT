CC=gcc
CFLAGS=-Wall -pedantic 
TARGETS=matrix prodcons condvar

all: $(TARGETS)

matrix: matrix.c
	$(CC) $(CFLAGS) -o matrix matrix.c -lpthread

condvar: condvar.c
	$(CC) $(CFLAGS) -o condvar condvar.c -lpthread

prodcons: prodcons.c
	$(CC) $(CFLAGS) -o prodcons prodcons.c -lpthread

clean:
	rm -f $(TARGETS) core *~
	
	
