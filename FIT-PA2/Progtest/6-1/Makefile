CXX=g++
LD=g++
CXXFLAGS=-std=c++11 -Wall -pedantic -g

all: dns

dns: solution.o
	$(LD) -g -o $@ $^

%.o:%cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o *~ *.bak core dns
	
solution.o: solution.cpp common.h tests.inc


