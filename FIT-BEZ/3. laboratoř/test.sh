#!/bin/bash

# Nacteni vstupu
file=$1

if [ ${#file} -eq 0 ]; then
	echo "Incorrect input!"
	exit 1
fi;

echo
echo "==================="
echo "3. Laborator"
echo "==================="
echo

echo "Running: make"
echo 

make

echo
echo "Running: ./encrypt cbc ./images/$1.bmp"
./encrypt cbc ./images/$1.bmp

echo "Running: ./decrypt cbc ./images/$1_cbc.bmp"
./decrypt cbc ./images/$1_cbc.bmp

echo "Running: ./encrypt ecb ./images/$1.bmp"
./encrypt ecb ./images/$1.bmp

echo "Running: ./decrypt ecb ./images/$1_ecb.bmp"
./decrypt ecb ./images/$1_ecb.bmp

echo
echo "==================="
echo "Success!"
echo "==================="
echo
