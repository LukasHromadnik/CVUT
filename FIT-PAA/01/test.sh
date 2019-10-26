#!/bin/bash

for size in 4 10 15 20 22 25 27 30 32 35 37 40
do
	echo $size
	time ./a.out $size &> /dev/null
done