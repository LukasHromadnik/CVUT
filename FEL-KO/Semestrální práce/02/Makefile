all: app

app:
	g++ -Wall -pedantic -std=c++11 main.cpp -o a.out

bundle: clean app
	zip -r0 coco02.zip main.cpp Source/*

clean:
	rm -f coco02.zip a.out