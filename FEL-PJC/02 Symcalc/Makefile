all: symcalc

symcalc:
	cmake --build build --target symcalc

clean:
	rm -f a.out
	rm -rf build/

prepare: clean
	cmake -Bbuild -H.

direct:
	cmake --build build --target tests-direct && ./build/tests-direct -a

runner:
	cmake --build build --target tests-runner-direct && ./build/tests-runner-direct -a

archive:
	zip -0 ~/Desktop/pjc02.zip *.cpp *.hpp
