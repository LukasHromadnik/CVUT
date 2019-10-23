#!/bin/bash

binary=${1:-par.o}

files=(graph10_5 graph17_3 graph20_3 graph25_3 graph10_6 graph12_5 graph14_4 graph18_4 graph14_5)

for file in ${files[*]}
do
	echo $file
	time ./$binary $file $2
	echo
done
