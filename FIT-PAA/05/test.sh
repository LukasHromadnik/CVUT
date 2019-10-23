#!/bin/bash

# Usage
# binary inputFile populationCount mutationProbability tournamentSize maxNumberOfGenerations childrenCount selectionStrategy crossbreedingStrategy reductionStrategy

# rm -f out.txt

instanceNumber=9
inputFilename="input/in_$instanceNumber.txt"
populationCount=50
mutationProbability=30
tournamentSize=25
maxNumberOfGenerations=150
childrenCount=20

#instanceNumber=9
#inputFilename="input/in_$instanceNumber.txt"
#populationCount=100
#mutationProbability=100
#tournamentSize=50
#maxNumberOfGenerations=300
#childrenCount=40

#instanceNumber=11
#inputFilename="input/in_$instanceNumber.txt"
#populationCount=1000
#mutationProbability=100
#tournamentSize=500
#maxNumberOfGenerations=300
#childrenCount=400

#for i in "partial" "stable"
#do
#	for m in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1 1.5 2
#	do
#		childrenCount=`echo $populationCount*$m | bc`
#		echo $childrenCount
#		for j in {1..5}
#		do
			time ./sat.o $inputFilename $populationCount $mutationProbability $tournamentSize $maxNumberOfGenerations $childrenCount "tournament" "uniform" "partial"
#		done
#	done
#done
