#!/bin/bash

./a.out < datapub/pub01.in > out01.txt
result=`diff out01.txt datapub/pub01.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub02.in > out02.txt
result=`diff out02.txt datapub/pub02.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub03.in > out03.txt
result=`diff out03.txt datapub/pub03.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub04.in > out04.txt
result=`diff out04.txt datapub/pub04.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub05.in > out05.txt
result=`diff out05.txt datapub/pub05.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub06.in > out06.txt
result=`diff out06.txt datapub/pub06.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub07.in > out07.txt
result=`diff out07.txt datapub/pub07.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub08.in > out08.txt
result=`diff out08.txt datapub/pub08.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub09.in > out09.txt
result=`diff out09.txt datapub/pub09.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub10.in > out10.txt
result=`diff out10.txt datapub/pub10.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub11.in > out11.txt
result=`diff out11.txt datapub/pub11.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

./a.out < datapub/pub12.in > out12.txt
result=`diff out12.txt datapub/pub12.out`
if [ "$result" != "" ]; then
    echo $result
else
    echo "OK"
fi;

rm -f out*
