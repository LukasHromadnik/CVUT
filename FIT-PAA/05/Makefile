GENERATOR_DIR=./Source/generator

all: sat generator

sat:
	g++ -std=c++11 main.cpp -o sat.o

generator:
	gcc $(GENERATOR_DIR)/g2.c -o generator.o

test:
	sh test.sh

clean:
	rm -f sat.o
	rm -f generator.o
