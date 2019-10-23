#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

#include "Source/loader.cpp"

using namespace std;

int main(int argc, char const *argv[]) {
	
	// Load input
	Input *input = new Input(argc, argv);

	// Setup random number generator
	setupRandomNumberGenerator();

	// Load knapsacks
	vector<Knapsack *> knapsacks = loadInstance(input->instanceSize);

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
	int iterationCount = 10;
	double sum = 0;
	
	for (Knapsack *knapsack : knapsacks) {
		for (int i = 0; i < iterationCount; i++) {
			EvolutionSolution *solution = evolution->solve(knapsack);
			sum += solution->m_Population->value();
		}

		cout << sum / (double)iterationCount << endl;
	}

	// for (Knapsack *knapsack : knapsacks) {
	// 	EvolutionSolution *solution = evolution->solve(knapsack);
	// 	cout << solution->m_Population->value() << endl;
	// 	cout << "=====" << endl;
	// }

	// for (int i = 0; i < iterationCount; i++) {
	// 	EvolutionSolution *solution = evolution->solve(knapsacks[input->instanceIndex]);
	// 	sum += solution->m_Population->value();
	// }

	// cout << sum / (double)iterationCount << endl;

	return 0;
}
