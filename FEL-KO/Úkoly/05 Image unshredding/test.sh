#!/bin/bash

for i in 01 02 03
do
    ./a.out datapub/$i.in.txt out.txt > /dev/null
    result=`diff datapub/$i.out.txt out.txt`

    if [ "$result" = "" ]; then
        echo "Test $i OK"
    else
        echo "Test $i FAIL"
        diff datapub/$i.out.txt out.txt
    fi
done
rm out.txt
