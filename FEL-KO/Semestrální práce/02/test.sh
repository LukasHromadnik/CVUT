#!/bin/bash

easy_test_folder="datapub"
hard_test_folder="datapub-hard"
test_folder=$easy_test_folder

for i in 00 01 02 03 04 05 06 07 08 09 10 11 12
# for i in 01 02 03 04 05 06 07 08 09
do
    ./a.out $test_folder/$i.in.txt out.txt 5 > /dev/null
    result=`diff out.txt $test_folder/$i.out.txt`
    if [ "$result" != "" ]; then
        echo "Test [$i] FAIL"
        cat out.txt | head -n 1
        cat $test_folder/$i.out.txt | head -n 1
    else
        echo "Test [$i] OK"
    fi;
done

rm out.txt
