#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <bitset>
#include <math.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ------------------------------------------------------------------------------------------------

struct KnapsackItem {
	int m_Price;
	int m_Volume;
	double getRatio() const;

	KnapsackItem();
	
	bool operator < (const KnapsackItem& other) const {
		return (this->getRatio() < other.getRatio());
	}
};

KnapsackItem::KnapsackItem() {
	m_Price = 0;
	m_Volume = 0;
}

double KnapsackItem::getRatio() const {
	return (double)m_Price / (double)m_Volume;
}

// ------------------------------------------------------------------------------------------------

struct Knapsack {
	int m_Identifier;
	int m_Capacity;
	vector<KnapsackItem *> m_Items;

	Knapsack();
};

Knapsack::Knapsack() {
	m_Identifier = 0;
	m_Capacity = 0;
	m_Items = vector<KnapsackItem *>();
}

// ------------------------------------------------------------------------------------------------

void printResult(Knapsack *knapsack, int price, vector<bool> mask) {
	cout << knapsack->m_Identifier << " " << knapsack->m_Items.size() << " " << price << "  ";
	for (int i = 0; i < mask.size(); i++) {
		cout << (mask[i] ? "1" : "0");
		if (i + 1 < mask.size()) {
			cout << " ";
		}

	}
	cout << endl;
}

// ------------------------------------------------------------------------------------------------

void solveUsingBrutalForce(Knapsack *knapsack) {
	int solutionPrice = 0;
	vector<bool> solutionMask;
	int numberOfItems = knapsack->m_Items.size();
	long long int numberOfCombinations = pow(2, numberOfItems);

	vector<bool> mask(knapsack->m_Items.size());
	for (long long int i = 0; i < numberOfCombinations; i++) {

		int resultPrice = 0;
		int resultVolume = 0;
		for (int index = 0; index < mask.size(); index++) {
			if (!mask[index]) { continue; }

			KnapsackItem *item = knapsack->m_Items[index];
			resultPrice += item->m_Price;
			resultVolume += item->m_Volume;
		}

		// Result
		if (resultPrice > solutionPrice && resultVolume <= knapsack->m_Capacity) {
			solutionPrice = resultPrice;
			solutionMask = mask;
		}

		if (i + 1 == numberOfCombinations) { break; }
		
		// Increment
		for (int j = 0; j < mask.size(); j++) {
			bool currentMask = mask[j];
			mask[j] = !mask[j];
			if (!currentMask) {
				break;
			}
		}
	}

	printResult(knapsack, solutionPrice, solutionMask);
}

void solveUsingHeuristic(Knapsack *knapsack) {
	int solutionPrice = 0;
	int solutionVolume = 0;
	vector<KnapsackItem *> items = knapsack->m_Items;
	vector<bool> mask(items.size(), 0);

	sort(items.begin(), items.end());

	for (int i = 0; i < items.size(); i++) {
		KnapsackItem *element = items[i];
		if (solutionVolume + element->m_Volume <= knapsack->m_Capacity) {
			solutionVolume += element->m_Volume;
			solutionPrice += element->m_Price;
			mask[i] = 1;
		}
	}

	printResult(knapsack, solutionPrice, mask);
}

// ------------------------------------------------------------------------------------------------

void usage() {
	cout << "Usage: knapsack [instance-size] [heuristic / brute-force]" << endl;
	cout << "---------------------" << endl;
	cout << "Possible values for instance-size are: 4, 10, 15, 20, 22, 25, 27, 30, 32, 35, 37, 40" << endl;
}

int main(int argc, const char * argv[])
{
	if (argc != 3) {
		usage();
		return 1;
	}

	int instanceSizes[] =  { 4, 10, 15, 20, 22, 25, 27, 30, 32, 35, 37, 40 };
	int fileNumber = stoi(argv[1]);
	bool found = false;
	for (int i = 0; i < (sizeof(instanceSizes)/sizeof(*instanceSizes)); i++) {
		if (instanceSizes[i] == fileNumber) {
			found = true;
			break;
		}
	}

	if (!found) {
		usage();
		return 1;
	}

	found = false;
	string solutionTypes[] = { "heuristic", "brute-force" };
	string solutionType = (string)argv[2];
	for (int i = 0; i < (sizeof(solutionTypes)/sizeof(*solutionTypes)); i++) {
		if (solutionTypes[i] == solutionType) {
			found = true;
			break;
		}
	}

	if (!found) {
		usage();
		return 1;
	}

	string line;
	string fileName = "./input/knap_" + (string)argv[1] + ".inst.dat";
	ifstream inputFile (fileName);

	if (inputFile.is_open()) {
		while (getline(inputFile, line)) {

			stringstream input(line);
			int tmp;
			vector<int> data;

			while (input >> tmp) {
				data.push_back(tmp);
			}	

			Knapsack *knapsack = new Knapsack();
			for (int i = 0; i < data.size(); i++) {
				if (i == 2) {
					knapsack->m_Identifier = data[0];
					knapsack->m_Capacity = data[2];
				} else if (i > 2 && i % 2 == 0) {
					KnapsackItem *item = new KnapsackItem();
					item->m_Price = data[i];
					item->m_Volume = data[i - 1];
					knapsack->m_Items.push_back(item);
				}
			}

			if (solutionType == "brute-force") {
				solveUsingBrutalForce(knapsack);
			} else if (solutionType == "heuristic") {
			// for (int i = 0; i < 10000; i++) {
				solveUsingHeuristic(knapsack);
			// }
			}
		}
	}

	return 0;
}
