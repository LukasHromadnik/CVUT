CXX=g++
CFLAGS=-Wall -Werror -g -pedantic -Wno-long-long -O0 -ggdb -std=c++11
OBJ=./hromalu1

all: compile doc

compile: main.o
	$(CXX) $< -o $(OBJ)

main.o: ./src/main.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

run:
	$(OBJ)

clean:
	rm -f $(OBJ)
	rm -f main.o
	rm -rf doc
	rm -f ./src/files/output.txt
	rm -f ./examples/example1/output.txt
	rm -f ./examples/example2/output.txt
	rm -f ./examples/example3/output.txt
	rm -f ./examples/example4/output.txt
	rm -f ./examples/example5/output.txt
	rm -f ./examples/example6/output.txt
	rm -f ./examples/example7/output.txt

doc:
	mkdir doc
	doxygen Doxyfile

example1:
	$(OBJ) $@

example2:
	$(OBJ) $@

example3:
	$(OBJ) $@

example4:
	$(OBJ) $@

example5:
	$(OBJ) $@

example6:
	$(OBJ) $@

example7:
	$(OBJ) $@
