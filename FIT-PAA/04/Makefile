KNAPGEN_DIR=./knapgen/

all: knapsack.o

knapsack.o:
	g++ -Wall -pedantic -std=c++11 main.cpp -o knapsack.o

test:
	sh test.sh

clean:
	rm -f knapsack.o
