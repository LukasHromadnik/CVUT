#!/bin/bash

file=${1:-main.cpp}

sshpass -p "*P4ssw0rd:cvut*" scp -r $file "hromalu1@star.fit.cvut.cz:/home/hromalu1/$file"
