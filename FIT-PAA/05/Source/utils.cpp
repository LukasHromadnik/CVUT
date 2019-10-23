#include <sys/time.h>
#include <fstream>
#include <sstream>

using namespace std;

void setupRandomNumberGenerator() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	srand(ms);
}

void loadProblem(Problem *problem, string line) {
	stringstream input(line);
	string tmp;
	vector<string> problemData;
	while (input >> tmp) {
		problemData.push_back(tmp);
	}
	int configurationSize = atoi(problemData[2].c_str());
	problem->setConfigurationSize(configurationSize);
}

void loadWeights(Problem *problem, string line) {
	stringstream input(line);
	string tmp;
	vector<string> data;
	while (input >> tmp) {
		data.push_back(tmp);
	}
	data.erase(data.begin());
	vector<int> weights;

	for (int i = 0; i < (int) data.size(); i++) {
		int value = atoi(data[i].c_str());
		weights.push_back(value);
	}
	problem->setWeights(weights);
}

Problem * loadInput(string fileName) {
	string line;
	ifstream inputFile (fileName);

	Problem *problem = new Problem();
	vector<Clause *> items;

	if (inputFile.is_open()) {
		vector<int> data;
		while (getline(inputFile, line)) {
			if (line[0] == 'c') {
				continue;
			} else if (line[0] == 'p') {
				loadProblem(problem, line);
			} else if (line[0] == 'w') {
				loadWeights(problem, line);
			} else {
				stringstream input(line);
				int tmp;
				while (input >> tmp) {
					if (tmp == 0) {
						Clause *clause = new Clause(data, 4);
						items.push_back(clause);
						data = vector<int>();
						continue;
					}
					data.push_back(tmp);
				}
			}
		}
	}

	inputFile.close();

	problem->setItems(items);

	return problem;
}
