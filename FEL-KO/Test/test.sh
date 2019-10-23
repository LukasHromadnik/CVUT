#!/bin/bash

for i in 01 02 03 04 05 06 07 08 09 10
do
    ./a.out datapub/in_$i.txt out.txt
    result=`diff datapub/out_$i.txt out.txt`
    if [ "$result" = "" ]; then
        echo "Test $i OK"
    else
        echo "Test $i FAIL"
        cat datapub/out_$i.txt
        echo
        echo "--"
        cat out.txt
        echo
    fi
done
rm out.txt
