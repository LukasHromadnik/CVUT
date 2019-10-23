all: clean problem app

problem:
	gcc -std=gnu99 -Wall -pedantic -c -o problem.o problem.c

app:
	g++ -std=c++11 -Wall -pedantic main.cpp problem.o

run:
	./a.out test/visitall/p-05-5.strips test/driverlog/pfile1.fdr

clean:
	rm -f problem.o a.out
