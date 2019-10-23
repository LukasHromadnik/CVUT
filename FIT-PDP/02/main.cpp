#include <iostream>
#include <set>
#include <math.h>
#include <vector>
#include <queue>
#include <sstream>
#include <fstream>
#include <omp.h>
#include "Timer.cpp"

using namespace std;

int indexFor(int i, int j, int numberOfNodes) {

	// Linearization of the two-dimensional space to single dimension
	return i * numberOfNodes + j;
}

void debug(vector<int> data, int numberOfNodes) {
	for (int i = 0; i < numberOfNodes; i++) {
		for (int j = 0; j < numberOfNodes; j++) {
			cout << data[indexFor(i, j, numberOfNodes)] << " ";
		}

		cout << endl;
	}
	cout << endl;
}

int numberOfEdges(vector<int> data) {

	// Sum of the indexes in the graph
	int sum = 0;

	// Iterate over all items in graph
	for (auto it : data) {

		// Increment sum with the value from given item
		sum += it;
	}
	
	// Graph is symmetrical, result is the half of the sum
	return sum / 2;
}

void isBipartite(int numberOfNodes, vector<int> graph, int &result) {

	// Initialize the queue of nodes
	queue<int> queue;

	// Initialize colors for each node
	int *colors = new int[numberOfNodes];

	// Set all colors to the undefined
	for (int i = 0; i < numberOfNodes; i++) {
		colors[i] = -1;
	}

	// Select first node
	int selectedNode = 0;

	// Set color for the selected node
	colors[selectedNode] = 0;

	// Push the selected node in the queue
	queue.push(selectedNode);

	// Iterate over every node
	while ( ! queue.empty()) {

		// Get the front node
		int node = queue.front();

		// Remove the front node from the queue
		queue.pop();

		// Iterate over each node
		for (int i = 0; i < numberOfNodes; i++) {

			int nodeIndex = indexFor(node, i, numberOfNodes);

			// Check if the selected node with given node have edge
			if (graph[nodeIndex] == 1 && colors[i] == -1) {

				// Set opposite color for the given node
				colors[i] = 1 - colors[node];

				// Push the given node in to the queue
				queue.push(i);

			}

			// If the selected node and the given node have edge and same colors
			// the graph is not bipartite
			else if (graph[nodeIndex] == 1 && colors[node] == colors[i]) {

				// Empty allocated memory
				delete[] colors;

				// Result for not bipartite graph
				result = 1;

				// No need to continue in processing
				return;
			}
		}
	}

	// Check if the graph is complete
	for (int i = 0; i < numberOfNodes; i++) {

		// If in colors array is negative number graph is not compelete
		if (colors[i] == -1) {

			// Empty allocated memory
			delete[] colors;

			// Result for not complete graph
			result = 2;

			// No need to continue in processing
			return;
		}
	}

	// Empty allocated memory
	delete[] colors;

	// Result for complete and bipartite graph
	result = 0;
}

bool canRemoveEdge(int numberOfNodes, vector<int> data) {

	// Check if the given graph has enough edges
	return (numberOfEdges(data) >= numberOfNodes);
}

void iteration(int numberOfNodes, vector<int> data, vector<int> &solution, int edgeX, int edgeY) {

	// Create helpers for edge counts
	int numberOfEdgesOfData = numberOfEdges(data);
	int numberOfEdgesOfSolution = numberOfEdges(solution);

	// Check if the given graph can have more edges than the current solution
	if (numberOfEdgesOfData <= numberOfEdgesOfSolution) return;

	// Result of the bipartite check
	int result;

	// Test if the graph is bipartite and complete
	isBipartite(numberOfNodes, data, result);

	// Graph is bipartite
	if (result == 0) {

		#pragma omp critical
		{
			// Check the solution again and then set the new solution
			if (numberOfEdgesOfData > numberOfEdges(solution)) {
				solution = data;
				cout << "[CRITICAL] " << numberOfEdgesOfData << endl;
			}
		}

		// No need to continue processing
		return;
	}

	// Graph is not complete, no need to remove more edges
	else if (result == 2) return;

	// Check if children can get better solution
	if (numberOfEdgesOfData - 1 <= numberOfEdgesOfSolution) return;

	// Check if the given graph has enough edges
	if ( ! canRemoveEdge(numberOfNodes, data)) return;

	// Iterate over all nodes
	// Set the first node to the x edge
	for (int i = edgeX; i < numberOfNodes; i++) {

		// Set the second node the y edge
		for (int j = edgeY; j < numberOfNodes; j++) {

			// Check if the index is in the upper right side from the diagonal
			if (j <= i) continue;

			// Check if given two nodes having a edge
			if (data[indexFor(i, j, numberOfNodes)] == 0) continue;

			// Copy current graph
			vector<int> newGraph = data;

			// Remove edge from the new graph
			newGraph[indexFor(i, j, numberOfNodes)] = 0;
			newGraph[indexFor(j, i, numberOfNodes)] = 0;

			#pragma omp task shared(solution)
			{
				// Run the iteration with new edges and graph
				iteration(numberOfNodes, newGraph, solution, i, j);
			}
		}

		// Reset the y edge for the next loop
		edgeY = i + 1;
	}
}

void loadInput(int numberOfParameters, const char **parameters, int &numberOfNodes, vector<int> &graph) {

	// Check parameters
	if (numberOfParameters != 2) {
		cout << "--------------------------" << endl;
		cout << "WRONG NUMBER OF PARAMETERS" << endl;
		cout << "Usage: binary input-file" << endl;
		cout << "--------------------------" << endl;
		exit(1);
	}

	// Set the input file's name
	string filename = "input/" + string(parameters[1]) + ".txt";

	// Load input file into the stream
	ifstream inFile(filename);

	// Helper property for given line of text
	string line;

	// Helper for counting the number of lines in the file
	int lineNumber = 0;

	// Iterate over each line of the file
	while (getline(inFile, line)) {

		// First line is the number of rows / cols in matrix
		if (lineNumber == 0) {

			// Load line of text into the stream
			istringstream ss(line);

			// Load the value in the property
			ss >> numberOfNodes;

		// Load given graph
		} else {

			// Load data from the row
			for (int i = 0; i < numberOfNodes; i++) {

				// Convert char number to int
				int edge = line[i] - '0';

				// Save edge to the graph
				graph.push_back(edge);
			}
		}

		// Update the line counter
		lineNumber++;

		// Do not load data out of bounds
		if (lineNumber > numberOfNodes) break;
	}
}

int main(int argc, const char * argv[]) {

	// Loaded number of nodes
	int numberOfNodes;

	// Loaded graph
	vector<int> graph;

	// Load input from a given file
	loadInput(argc, argv, numberOfNodes, graph);

	// Prepare solution variable
	vector<int> solution (pow(numberOfNodes, 2), 0);

	Timer runTimer("finish computation");

	#pragma omp parallel
	{
		#pragma omp single
		{
			// Run the iterative algorithm
			iteration(numberOfNodes, graph, solution, 0, 1);
		}
	}

	// #pragma omp taskwait

	// Print the founded solution
	debug(solution, numberOfNodes);

	// Print the number of edges of the solution
	cout << numberOfEdges(solution) << endl;

	runTimer.end();

	return 0;
}
