CC=gcc
CFLAGS=-Wall -pedantic -ansi 
LIBS=-lpthread  
CXX=g++
CXXFLAGS=-Wall -pedantic

all:	01_thr 02_thr 03_thr 04_thr

01_thr: 01_thr.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

02_thr: 02_thr.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

03_thr: 03_thr.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

04_thr: 04_thr.cpp
	$(CXX) -std=c++11 $(CXXFLAGS) -o $@ $< $(LIBS)
clean:
	\rm -f 01_thr 02_thr 03_thr 04_thr *~ core
