PRE=OMPI_CXX=g++-6
CC=mpic++
CFLAGS=-Wall -pedantic -std=c++11 -fopenmp -O3
INPUT=main.cpp

all: app

app:
	$(PRE) $(CC) $(CFLAGS) $(INPUT) -o $@.o

test:
	$(PRE) $(CC) $(CFLAGS) $@.cpp -o $@.o

clean:
	rm -f app.o
