KNAPGEN_DIR=./knapgen/

all: knapgen.o knapsack.o

knapsack.o:
	g++ -Wall -pedantic -std=c++11 main.cpp -o knapsack.o

knapgen.o:
	gcc -lm $(KNAPGEN_DIR)/knapgen.c $(KNAPGEN_DIR)/knapcore.c -o knapgen.o

test:
	sh test.sh

clean:
	rm -f knapsack.o
	rm -f knapgen.o
	rm -f in.txt
