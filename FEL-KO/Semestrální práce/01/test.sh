#!/bin/bash

for i in 00 01 02 03 04 05 06 07 08 09 10 11 12
do
    ./a.out datapub/$i.in.txt out$i.txt
    result=`diff out$i.txt datapub/$i.out.txt`
    if [ "$result" != "" ]; then
        echo "Test [$i] FAIL"
        cat out$i.txt
        echo
        cat datapub/$i.out.txt
        echo
    else
        echo "Test [$i] OK"
    fi;
done

rm out*
