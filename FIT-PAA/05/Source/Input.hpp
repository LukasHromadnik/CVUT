#include <string>

using namespace std;

struct Input {
	string inputFile;

	int populationCount;
	int mutationProbability;
	int tournamentSize;
	int maxNumberOfGenerations;
	int childrenCount;

	string selectionStrategy;
	string crossbreedingStrategy;
	string reductionStrategy;

	Input(int numberOfArguments, char const *arguments[]);
};

Input::Input(int numberOfArguments, char const *arguments[]) {
	if (numberOfArguments != 10) {
		cout << "binary inputFile populationCount mutationProbability tournamentSize maxNumberOfGenerations childrenCount selectionStrategy crossbreedingStrategy reductionStrategy" << endl;
		exit(0);
	}

	inputFile = arguments[1];

	populationCount = atoi(arguments[2]);
	mutationProbability = atoi(arguments[3]);
	tournamentSize = atoi(arguments[4]);
	maxNumberOfGenerations = atoi(arguments[5]);
	childrenCount = atoi(arguments[6]);

	selectionStrategy = arguments[7];
	crossbreedingStrategy = arguments[8];
	reductionStrategy = arguments[9];
}
