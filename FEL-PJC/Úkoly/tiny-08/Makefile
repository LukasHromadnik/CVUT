all: prepare app run

prepare:
	cmake -Bbuild -H.

app:
	cmake --build build

run:
	./build/tests
