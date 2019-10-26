CXX=g++
CFLAGS=-Wall -pedantic
LIBS=-lcrypto

all: encrypt decrypt

encrypt: encrypt.cpp
	$(CXX) $(CFLAGS) -o $@ $< $(LIBS)

decrypt: decrypt.cpp
	$(CXX) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm encrypt decrypt
