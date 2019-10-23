#include <string>

using namespace std;

struct Input {
	int instanceSize;

	int populationCount;
	int mutationProbability;
	int tournamentSize;
	int maxNumberOfGenerations;
	int childrenCount;

	string selectionStrategy;
	string crossbreedingStrategy;
	string reductionStrategy;

	int instanceIndex;

	Input(int numberOfArguments, char const *arguments[]);
};

Input::Input(int numberOfArguments, char const *arguments[]) {
	if (numberOfArguments != 11) {
		cout << "binary instance populationCount mutationProbability tournamentSize maxNumberOfGenerations childrenCount selectionStrategy crossbreedingStrategy reductionStrategy instanceIndex" << endl;
		exit(0);
	}

	instanceSize = atoi(arguments[1]);

	populationCount = atoi(arguments[2]);
	mutationProbability = atoi(arguments[3]);
	tournamentSize = atoi(arguments[4]);
	maxNumberOfGenerations = atoi(arguments[5]);
	childrenCount = atoi(arguments[6]);

	selectionStrategy = arguments[7];
	crossbreedingStrategy = arguments[8];
	reductionStrategy = arguments[9];

	instanceIndex = atoi(arguments[10]);
}
