#!/bin/bash

binary=${4:-app.o}

TMPDIR=/tmp mpirun ./$binary $1 $2 $3
