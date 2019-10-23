CC=g++-6
CFLAGS=-Wall -pedantic -std=c++11 -O3 -fopenmp
INPUT=main.cpp

all: app

app:
	$(CC) $(CFLAGS) $(INPUT) -o $@.o

clean:
	rm -f app.o
