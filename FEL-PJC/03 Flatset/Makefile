all:
	cmake --build build

prepare: clean
	cmake -Bbuild -H.

clean:
	rm -rf build

basic:
	cmake --build build --target tests-basic && ./build/tests-basic

full:
	cmake --build build --target tests-full && ./build/tests-full -a

compile:
	g++ -std=c++14 flatset.hpp

archive:
	zip -0 ~/Desktop/pjc03.zip flatset.hpp

test:
	g++ -Wall -pedantic --std=c++14 main.cpp
