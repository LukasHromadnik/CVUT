#!/bin/bash

# Usage
# binary instance populationCount mutationProbability tournamentSize maxNumberOfGenerations childrenCount selectionStrategy crossbreedingStrategy reductionStrategy instanceIndex

# rm -f out.txt

instance=40
populationCount=50
mutationProbability=30
tournamentSize=10
maxNumberOfGenerations=80
childrenCount=50

for i in 10 20 30 40 50 60 70 80 90 100
do
		./a.out $instance $populationCount $mutationProbability $tournamentSize $maxNumberOfGenerations $childrenCount "roulette" "uniform" "partial" 0 > "out_${i}.txt"
done

# paste `awk 'BEGIN { for (i = 0; i <= 49; i++) print "out_" i ".txt" }'` | column -s '\t' -t > out.txt
# rm -f `awk 'BEGIN { for (i = 0; i <= 49; i++) print "out_" i ".txt" }'`