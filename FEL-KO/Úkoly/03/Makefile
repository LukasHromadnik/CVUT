all: clean app

app:
	g++ -std=c++11 -Wall -pedantic main.cpp -o a.out

run:
	./a.out in.txt out.txt

bundle: app
	zip -0 ko03.zip main.cpp Source/*

clean:
	rm -f a.out ko03.zip

test: app
	sh test.sh
