#!/bin/bash

for i in 01 02 03 04
do
    ./a.out datapub/$i.in.txt out.txt
    result=`awk '{print NF}' out.txt`
    result_test=`awk '{print NF}' datapub/$i.out.txt`

    if [ "$result" = "$result_test" ]; then
        echo "Test $i OK"
    else
        echo "Test $i FAIL"
    fi
done
rm out.txt
