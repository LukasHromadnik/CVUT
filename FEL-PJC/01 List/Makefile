all: s1 s2 s3 s4

prepare:
	cmake -Bbuild -H.

s1:
	cmake --build build --target tests-stage-1
	./build/tests-stage-1

s2:
	cmake --build build --target tests-stage-2
	./build/tests-stage-2

s3:
	cmake --build build --target tests-stage-3
	./build/tests-stage-3

s4:
	cmake --build build --target tests-stage-4
	./build/tests-stage-4

s5:
	cmake --build build --target tests-stage-5 --config Release
	./build/tests-stage-5
