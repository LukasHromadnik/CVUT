#!/usr/bin/env bash

# Download and compile LKH Solver

#VERSION=2.0.7
VERSION=2.0.9
FILE="LKH-$VERSION.tgz"

curl -o "$FILE" "http://akira.ruc.dk/~keld/research/LKH/$FILE"
tar xvfz LKH-$VERSION.tgz
cd LKH-$VERSION
make
