#!/bin/bash

for test in 01 02 03 04 05 06 07 08 09 10
do
    ./a.out < "datapub/pub$test.in" > result
    echo `diff result "datapub/pub$test.out"`
done
rm result
