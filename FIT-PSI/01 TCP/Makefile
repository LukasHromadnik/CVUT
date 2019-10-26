CXX=g++
CFLAGS=-Wall -pedantic

all: server

server: server.cpp
	$(CXX) $(CFLAGS) -o $@ $<

client: client.cpp
	$(CXX) $(CFLAGS) -o $@ $<

clean:
	rm server client
