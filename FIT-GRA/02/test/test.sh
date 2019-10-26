#!/bin/bash

cd data

rm -f *_my.txt

testdiff() {
	../a.out "$1_in.txt" "$1_my.txt"
	echo -n "Test $1.txt ... "
	if [ -n "$2" ]; then
		diff "$1"_out.txt "$1"_my.txt >/dev/null 2>&1 && echo -e "OK" || echo -e "FAIL"
	else
		diff "$1"_out.txt "$1"_my.txt && echo -e "OK" || echo -e "FAIL"
	fi
}

number_of_files=$(expr `ls -l | wc -l` - 1 / 2)
number_of_tests=$(expr $number_of_files / 2)

for (( i=1; i<=$number_of_tests; i++ ))
do
	testdiff $i
done

cd ..
