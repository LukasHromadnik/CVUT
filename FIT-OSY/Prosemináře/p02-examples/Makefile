CC=gcc
CFLAGS=-Wall -pedantic 
PRIMELIBS=-lpthread -lm
TRAINLIBS=-lpthread -lrt
MATRIXLIBS=-lpthread
TARGETS=prime1 prime2 prime3 prime4 prime5 prime6 prime7 prime8 train1 train2 train3 train4 matrix
CXX=g++
CXXFLAGS=-std=c++11 -Wall -pedantic 

all: $(TARGETS)

prime1: prime1.c
	$(CC) $(CFLAGS) -lm -o $@ $<
	
prime2: prime2.c
	$(CC) $(CFLAGS) $(PRIMELIBS) -o $@ $<

prime3: prime3.c
	$(CC) $(CFLAGS) $(PRIMELIBS) -o $@ $<

prime4: prime4.c
	$(CC) $(CFLAGS) $(PRIMELIBS) -o $@ $<

prime5: prime5.c
	$(CC) $(CFLAGS) $(PRIMELIBS) -o $@ $<

prime6: prime6.c
	$(CC) $(CFLAGS) $(PRIMELIBS) -o $@ $<

prime7: prime7.c
	$(CC) $(CFLAGS) $(PRIMELIBS) -o $@ $<

prime8: prime8.cpp
	$(CXX) $(CXXFLAGS) $(PRIMELIBS) -o $@ $<

train1: train1.c
	$(CC) $(CFLAGS) $(TRAINLIBS) -o $@ $<

train2: train2.c
	$(CC) $(CFLAGS) $(TRAINLIBS) -o $@ $<

train3: train3.c
	$(CC) $(CFLAGS) $(TRAINLIBS) -o $@ $<

train4: train4.cpp
	$(CXX) $(CXXFLAGS) $(TRAINLIBS) -o $@ $<

matrix: matrix.c
	$(CC) $(CFLAGS) $(MATRIXLIBS) -o $@ $<

clean:
	rm -f $(TARGETS) core *~
	
	