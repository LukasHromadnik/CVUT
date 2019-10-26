CXX=g++
CXXFLAGS= -std=c++11 -Wall -pedantic -g
#CXXFLAGS=-Wall -pedantic -g 
LIBS=-lpthread

all: seqtest speedtest thrtest

seqtest: seqmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

speedtest: speedmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

thrtest: thrmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f core *~ *.o seqtest speedtest thrtest
	rm -rf *.dSYM
