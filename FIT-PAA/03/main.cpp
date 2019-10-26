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

#include "KnapsackItem.hpp"
#include "Knapsack.hpp"
#include "KnapsackSolution.hpp"

using namespace std;

// ------------------------------------------------------------------------------------------------

KnapsackSolution * max(KnapsackSolution *left, KnapsackSolution *right) {
	return left->m_Price > right->m_Price ? left : right;
}

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

KnapsackSolution * recursiveKnapsack(int i, int w, int price, Knapsack *knapsack) {
	int numberOfItems = knapsack->m_Items.size();

	if (i == numberOfItems || w <= 0) {
		return new KnapsackSolution(knapsack->m_Items.size());
	}

	KnapsackItem *item = knapsack->m_Items[i];
	int itemPrice = item->m_Price;
	int itemWeight = item->m_Volume;

	if (itemWeight > w) {
		KnapsackSolution *result = recursiveKnapsack(i + 1, w, price, knapsack);
		result->m_Items[i] = 0;
		return result;
	} else {
		KnapsackSolution *withResult = recursiveKnapsack(i + 1, w - itemWeight, price + itemPrice, knapsack);
		withResult->m_Price += itemPrice;
		withResult->m_Items[i] = 1;

		// int sumOfItems = 0;
		// for (int j = i + 1; j < numberOfItems; j++) {
		// 	sumOfItems += knapsack->m_Items[j]->m_Price;
		// }

		// if (withResult->m_Price > price + sumOfItems) {
		// 	return withResult;
		// }

		KnapsackSolution *withoutResult = recursiveKnapsack(i + 1, w, price, knapsack);
		withoutResult->m_Items[i] = 0;

		return max(withoutResult, withResult);
	}
}

void solveUsingBrutalForce(Knapsack *knapsack) {
	KnapsackSolution *solution = recursiveKnapsack(0, knapsack->m_Capacity, 0, knapsack);
	printResult(knapsack, solution->m_Price, solution->m_Items);
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
// ------------------------------------------------------------------------------------------------

KnapsackSolution * recursiveKnapsackBB(int i, int w, int price, Knapsack *knapsack) {
	int numberOfItems = knapsack->m_Items.size();

	if (i == numberOfItems || w == 0) {
		return new KnapsackSolution(knapsack->m_Items.size());
	}

	KnapsackItem *item = knapsack->m_Items[i];
	int itemPrice = item->m_Price;
	int itemWeight = item->m_Volume;

	if (itemWeight > w) {
		KnapsackSolution *result = recursiveKnapsackBB(i + 1, w, price, knapsack);
		result->m_Items[i] = 0;
		return result;
	} else {
		KnapsackSolution *withResult = recursiveKnapsackBB(i + 1, w - itemWeight, price + itemPrice, knapsack);
		withResult->m_Price += itemPrice;
		withResult->m_Items[i] = 1;

		int sumOfItems = 0;
		for (int j = i + 1; j < numberOfItems; j++) {
			sumOfItems += knapsack->m_Items[j]->m_Price;
		}

		if (withResult->m_Price > price + sumOfItems) {
			return withResult;
		}

		KnapsackSolution *withoutResult = recursiveKnapsackBB(i + 1, w, price, knapsack);
		withoutResult->m_Items[i] = 0;

		return max(withoutResult, withResult);
	}
}

void solveUsingBranchBound(Knapsack *knapsack) {
	KnapsackSolution *solution = recursiveKnapsackBB(0, knapsack->m_Capacity, 0, knapsack);
	printResult(knapsack, solution->m_Price, solution->m_Items);
}

void solveUsingDynamic(Knapsack *knapsack) {

	// Set helper variable
	int numberOfItems = knapsack->m_Items.size();
	
	// Get the last index for price axis
	int sumOfPrices = 0;
	for (int i = 0; i < numberOfItems; i++) {
		sumOfPrices += knapsack->m_Items[i]->m_Price;
	}

	// Set bounds for loops
	int itemsAxisLimit = numberOfItems + 1;
	int pricesAxisLimit = sumOfPrices + 1;

	// Init result table
	int** result = new int*[itemsAxisLimit];
	for (int i = 0; i < itemsAxisLimit; i++) {
		result[i] = new int[pricesAxisLimit];

		// Set the default values
		for (int j = 0; j < pricesAxisLimit; j++) {
			result[i][j] = -1;
		}
	}

	// Set the first item for the algorithm
	result[0][0] = 0;

	// Iterate over items
	for (int i = 0; i < numberOfItems; i++) {

		// For each item compute the next value with or without the selected item
		for (int j = 0; j < pricesAxisLimit; j++) {

			// Set current item value
			int currentValue = result[i][j];

			// Nothing to compute here
			if (currentValue == -1) continue;

			// Set helper variables
			KnapsackItem *item = knapsack->m_Items[i];
			int weight = item->m_Volume;
			int price = item->m_Price;

			// Combination with the selected item
			result[i + 1][j + price] = currentValue + weight;

			// Combination without selected item
			if (result[i + 1][j] == -1 || result[i + 1][j] > currentValue) {
				result[i + 1][j] = currentValue;
			}
		}
	}

	// Debug
	/*
	for (int j = sumOfPrices + 1; j >= 0; j--) {
		cout << setfill(' ') << setw(4) << j << "| ";
		for (int i = 0; i < numberOfItems + 1; i++) {
			if (result[i][j] == -1) {
				cout << "   ";
			}
			else {
				cout << setfill(' ') << setw(4) << result[i][j];
			}
		}
		cout << endl;
	}
	cout << "-----------------------------" << endl;
	cout << "    | ";
	for (int i = 0; i < numberOfItems + 1; i++) {
		cout << setfill(' ') << setw(4) << i;
	}
	cout << endl;
	*/

	// Finding the best combination
	//
	// Search from the top of the last column first item with value under or equal to the knapsack capacity is the result

	// Initalize solution variable
	int solutionPrice = 0;

	// Iterate over prices from the top
	for (int i = sumOfPrices; i >= 0; i--) {

		// Current solution value
		int solution = result[numberOfItems][i];

		// If the solution is above the knapsack capacity or it's not computed
		if (solution > knapsack->m_Capacity || solution == -1) continue;

		// Searched value is the "index" that gets here first
		solutionPrice = i;

		// No need to search for better solution
		break;
	}

	// Mask vector
	vector<bool> mask = vector<bool>(numberOfItems, 0);

	// Set value for the iteration
	int solutionIndex = solutionPrice;

	// Iterate over items from the back
	for (int i = numberOfItems; i > 0; i--) {

		// Get current value
		int currentValue = result[i][solutionIndex];

		// If the current value is equal to the previous value the item is not included in the optimal solution
		if (result[i - 1][solutionIndex] == currentValue) continue;

		// Get the price of the previous item
		int price = knapsack->m_Items[i - 1]->m_Price;

		// Shift the value of the price index
		solutionIndex -= price;

		// Item is included in the optimal solution
		mask[i - 1] = 1;
	}

	// Print the formatted result
	printResult(knapsack, solutionPrice, mask);

	// Empty the memory
	for (int i = 0; i < itemsAxisLimit; i++) {
		delete[] result[i];
	}
	delete[] result;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

void solveUsingFPTAS(Knapsack *knapsack, double relativeError) {

	// Set helper variable
	int numberOfItems = knapsack->m_Items.size();
	
	// Get max price
	int maxPrice = 0;
	for (int i = 0; i < numberOfItems; i++) {
		int itemPrice = knapsack->m_Items[i]->m_Price;
		if (itemPrice > maxPrice) {
			maxPrice = itemPrice;
		}
	}

	// Relative error
	// double relativeError = 0.5;
	double K = (relativeError * maxPrice) / numberOfItems;

	vector<KnapsackItem *> newItems = knapsack->m_Items;
	for (int i = 0; i < newItems.size(); i++) {
		newItems[i]->m_Price = floor(newItems[i]->m_Price / K);
	}
	
	// Get the last index for price axis
	int sumOfPrices = 0;
	for (int i = 0; i < numberOfItems; i++) {
		sumOfPrices += newItems[i]->m_Price;
	}

	// Set bounds for loops
	int itemsAxisLimit = numberOfItems + 1;
	int pricesAxisLimit = sumOfPrices + 1;

	// Init result table
	int** result = new int*[itemsAxisLimit];
	for (int i = 0; i < itemsAxisLimit; i++) {
		result[i] = new int[pricesAxisLimit];

		// Set the default values
		for (int j = 0; j < pricesAxisLimit; j++) {
			result[i][j] = -1;
		}
	}

	// Set the first item for the algorithm
	result[0][0] = 0;

	// Iterate over items
	for (int i = 0; i < numberOfItems; i++) {

		// For each item compute the next value with or without the selected item
		for (int j = 0; j < pricesAxisLimit; j++) {

			// Set current item value
			int currentValue = result[i][j];

			// Nothing to compute here
			if (currentValue == -1) continue;

			// Set helper variables
			KnapsackItem *item = newItems[i];
			int weight = item->m_Volume;
			int price = item->m_Price;

			// Combination with the selected item
			result[i + 1][j + price] = currentValue + weight;

			// Combination without selected item
			if (result[i + 1][j] == -1 || result[i + 1][j] > currentValue) {
				result[i + 1][j] = currentValue;
			}
		}
	}

	// Debug
	/*
	for (int j = sumOfPrices + 1; j >= 0; j--) {
		cout << setfill(' ') << setw(4) << j << "| ";
		for (int i = 0; i < numberOfItems + 1; i++) {
			if (result[i][j] == -1) {
				cout << "   ";
			}
			else {
				cout << setfill(' ') << setw(4) << result[i][j];
			}
		}
		cout << endl;
	}
	cout << "-----------------------------" << endl;
	cout << "    | ";
	for (int i = 0; i < numberOfItems + 1; i++) {
		cout << setfill(' ') << setw(4) << i;
	}
	cout << endl;
	*/

	// Finding the best combination
	//
	// Search from the top of the last column first item with value under or equal to the knapsack capacity is the result

	// Initalize solution variable
	int solutionPrice = 0;

	// Iterate over prices from the top
	for (int i = sumOfPrices; i >= 0; i--) {

		// Current solution value
		int solution = result[numberOfItems][i];

		// If the solution is above the knapsack capacity or it's not computed
		if (solution > knapsack->m_Capacity || solution == -1) continue;

		// Searched value is the "index" that gets here first
		solutionPrice = i;

		// No need to search for better solution
		break;
	}

	// Mask vector
	vector<bool> mask = vector<bool>(numberOfItems, 0);

	// Set value for the iteration
	int solutionIndex = solutionPrice;

	// Iterate over items from the back
	for (int i = numberOfItems; i > 0; i--) {

		// Get current value
		int currentValue = result[i][solutionIndex];

		// If the current value is equal to the previous value the item is not included in the optimal solution
		if (result[i - 1][solutionIndex] == currentValue) continue;

		// Get the price of the previous item
		int price = knapsack->m_Items[i - 1]->m_Price;

		// Shift the value of the price index
		solutionIndex -= price;

		// Item is included in the optimal solution
		mask[i - 1] = 1;
	}

	// Print the formatted result
	printResult(knapsack, solutionPrice, mask);

	// Empty the memory
	for (int i = 0; i < itemsAxisLimit; i++) {
		delete[] result[i];
	}
	delete[] result;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------



void usage() {
	cout << "Usage: knapsack [instance-size] [computation type] [relative error]?" << endl;
	cout << "---------------------" << endl;
	cout << "Possible values for instance-size are: 4, 10, 15, 20, 22, 25, 27, 30, 32, 35, 37, 40" << endl;
	cout << "Possible values for capomutation type are: brute-force, heuristic, bb (branch & bound), dynamic, ftpas" << endl;
}

int main(int argc, const char * argv[])
{
	// if (argc != 3) {
	// 	usage();
	// 	return 1;
	// }

	int instanceSizes[] =  { 4, 10, 15, 20, 22, 25, 27, 30, 32, 35, 37, 40 };
	int fileNumber = stoi(argv[1]);
	bool found = false;
	for (int i = 0; i < (sizeof(instanceSizes) / sizeof(*instanceSizes)); i++) {
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
	string solutionTypes[] = { "heuristic", "brute-force", "bb", "dynamic", "fptas" };
	string solutionType = (string)argv[2];
	for (int i = 0; i < (sizeof(solutionTypes) / sizeof(*solutionTypes)); i++) {
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
	// string fileName = "./input/knap_" + (string)argv[1] + ".inst.dat";
	string fileName = "in.txt";
	ifstream inputFile (fileName);
	// string solutionType = "fptas";
	int counter = 0;
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
			
			counter++;

			if (solutionType == "brute-force") {
				solveUsingBrutalForce(knapsack);
				if (knapsack->m_Items.size() == 25 && counter == 10) break;
				if (knapsack->m_Items.size() == 30 && counter == 1) break;
			} else if (solutionType == "heuristic") {
				solveUsingHeuristic(knapsack);
			} else if (solutionType == "bb") {
				solveUsingBranchBound(knapsack);
				if (knapsack->m_Items.size() == 25 && counter == 10) break;
				if (knapsack->m_Items.size() == 30 && counter == 1) break;
			} else if (solutionType == "dynamic") {
				for (int j = 0; j < 100; j++) {
					solveUsingDynamic(knapsack);
				}
			} else if (solutionType == "fptas") {
				double relativeError = atof(argv[3]);
				for (int j = 0; j < 100; j++) {
					solveUsingFPTAS(knapsack, relativeError);
				}
			}
		}
	}

	return 0;
}
