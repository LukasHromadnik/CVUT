CXX=g++
CFLAGS=-Wall -pedantic
LIBS=-lcrypto -lssl

all: main

main: main.cpp
	$(CXX) $(CFLAGS) -o $@ $< $(LIBS)

cert:
	openssl x509 -noout -in cert.pem -text

clean:
	rm main cert.pem index.html main.cpp~
