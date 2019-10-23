#!/bin/bash

sum=0
for i in 1 2 3 4 5
do
    python main.py > result
    average=`cat result | tail -n 1`
    new_sum="$sum + $average"
    sum=`bc <<< $new_sum`
    echo $average
done

result="scale=4;$sum / 5"
echo "-----"
echo "Average" `bc -l <<< $result`
