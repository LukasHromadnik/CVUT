#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <limits.h>

#include "Source/loader.cpp"

using namespace std;

int main(int argc, char const *argv[]) {

	// Setup random number generator
	setupRandomNumberGenerator();

	// Load input
	Input *input = new Input(argc, argv);

	// Load SAT problem
	Problem *problem = loadInput(input->inputFile);

	// Selection strategy
	SelectionStrategy *selectionStrategy = NULL;
	if (input->selectionStrategy == "roulette") {
		selectionStrategy = new RouletteSelectionStrategy();
	} else if (input->selectionStrategy == "tournament") {
		selectionStrategy = new TournamentSelectionStrategy(input->tournamentSize);
	}

	// Crossbreeding strategy
	CrossbreedingStrategy *crossbreedingStrategy = NULL;
	if (input->crossbreedingStrategy == "single") {
		crossbreedingStrategy = new SinglePointCrossbreedingStrategy();
	} else if (input->crossbreedingStrategy == "double") {
		crossbreedingStrategy = new DoublePointCrossbreedingStrategy();
	} else if (input->crossbreedingStrategy == "uniform") {
		crossbreedingStrategy = new UniformCrossbreedingStrategy();
	}
	
	// Reduction strategy
	ReductionStrategy *reductionStrategy = NULL;
	if (input->reductionStrategy == "partial") {
		reductionStrategy = new PartialReductionStrategy(input->childrenCount);
	} else if (input->reductionStrategy == "substitution") {
		reductionStrategy = new SubstitutionReductionStrategy();
	} else if (input->reductionStrategy == "stable") {
		reductionStrategy = new StableReductionStrategy(input->childrenCount);
	}

	// Create evolution
	Evolution *evolution = new Evolution(selectionStrategy, crossbreedingStrategy, reductionStrategy, input->populationCount, input->mutationProbability, input->maxNumberOfGenerations);

	// Run the algorithm
	EvolutionSolution *solution = evolution->solve(problem);
	solution->debug();
	// evolution->solve(problem);

	return 0;
}
