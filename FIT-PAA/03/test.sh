 #!/bin/bash

for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1 # poměr
# for i in 100 250 500 1000 10000 # max weight
# for i in 100 250 500 1000 2500 5000 # max cost
# for i in -1 0 1
do
	echo $i
	./knapgen.o -n 50 -N 50 -m $i -W 100 -C 300 -k 1 -d 0 > in.txt 2> /dev/null

	for type in "dynamic"
	do
		echo $type
		# filename=$type"_"$i".txt"
		filename=/dev/null
		time ./knapsack.o 15 $type
		# cat $filename | cut -d ' ' -f 3 > $filename".test.txt"
		# time ./knapsack.o 15 $type | cut -d ' ' -f 3 > $filename
		# time ./a.out 15 $type
	done
	echo
done
