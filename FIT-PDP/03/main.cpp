#include <iostream>
#include <set>
#include <math.h>
#include <vector>
#include <deque>
#include <queue>
#include <sstream>
#include <fstream>
#include <omp.h>

#include "Timer.cpp"

#define GRAPH_BIPARTITE_COMPLETE 0
#define GRAPH_NOT_BIPARTITE 1
#define GRAPH_NOT_COMPLETE 2

#define SOLUTION_FOUND 1
#define SOLUTION_NOT_FOUND 0

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Problem {
	Problem(vector<int> graph, int edgeX, int edgeY);
	vector<int> graph;
	int edgeX;
	int edgeY;
};

Problem::Problem(vector<int> graph, int edgeX, int edgeY) {
	this->graph = graph;
	this->edgeX = edgeX;
	this->edgeY = edgeY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int indexFor(int i, int j, int numberOfNodes) {

	// Linearization of the two-dimensional space to the single dimension
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

int isBipartite(int numberOfNodes, vector<int> graph) {

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

				// Return for not bipartite graph
				return GRAPH_NOT_BIPARTITE;
			}
		}
	}

	// Check if the graph is complete
	for (int i = 0; i < numberOfNodes; i++) {

		// If in colors array is negative number graph is not compelete
		if (colors[i] == -1) {

			// Empty allocated memory
			delete[] colors;

			// Return for not complete graph
			return GRAPH_NOT_COMPLETE;
		}
	}

	// Empty allocated memory
	delete[] colors;

	// Return for complete and bipartite graph
	return GRAPH_BIPARTITE_COMPLETE;
}

bool canRemoveEdge(int numberOfNodes, vector<int> data) {

	// Check if the given graph has enough edges
	return (numberOfEdges(data) >= numberOfNodes);
}

void printSolution(int numberOfNodes, vector<int> solution) {

	// Print the founded solution
	debug(solution, numberOfNodes);

	// Print the number of edges of the solution
	cout << numberOfEdges(solution) << endl;
}

int iterationSeq(int numberOfNodes, vector<int> data, vector<int> &solution) {

	// Create helpers for edge counts
	int numberOfEdgesOfData = numberOfEdges(data);
	int numberOfEdgesOfSolution = numberOfEdges(solution);

	// Check if the given graph can have more edges than the current solution
	if (numberOfEdgesOfData <= numberOfEdgesOfSolution) return 0;

	// Test if the graph is bipartite and complete
	int result = isBipartite(numberOfNodes, data);

	// Graph is bipartite and has more edges than the solution
	if (result == GRAPH_BIPARTITE_COMPLETE) {
		
		// Set the new solution
		solution = data;

		// Return value for new solution
		return SOLUTION_FOUND;
	}

	// Graph did not update the solution
	return SOLUTION_NOT_FOUND;
}

void iteration(int numberOfNodes, vector<int> data, vector<int> &solution, int edgeX, int edgeY) {

	// Create helpers for edge counts
	int numberOfEdgesOfData = numberOfEdges(data);
	int numberOfEdgesOfSolution = numberOfEdges(solution);

	// Check if the given graph can have more edges than the current solution
	if (numberOfEdgesOfData <= numberOfEdgesOfSolution) return;

	// Test if the graph is bipartite and complete
	int result = isBipartite(numberOfNodes, data);

	// Graph is bipartite
	if (result == 0) {

		#pragma omp critical
		{
			// Check the solution again and then set the new solution
			if (numberOfEdgesOfData > numberOfEdges(solution)) {
				solution = data;
				// cout << "[CRITICAL] " << numberOfEdgesOfData << endl;
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

			// Run the iteration with new edges and graph
			iteration(numberOfNodes, newGraph, solution, i, j);
		}

		// Reset the y edge for the next loop
		edgeY = i + 1;
	}
}

void loadInput(int numberOfParameters, const char **parameters, int &numberOfNodes, vector<int> &graph, int &maxDepth) {

	// Check parameters
	if (numberOfParameters != 3) {
		cout << "--------------------------" << endl;
		cout << "WRONG NUMBER OF PARAMETERS" << endl;
		cout << "Usage: binary input-file max-depth" << endl;
		cout << "--------------------------" << endl;
		exit(1);
	}

	// Load max depth from cmd
	maxDepth = atoi(parameters[2]);

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

	// Max depth limit
	int maxDepth;

	// Load input from a given file
	loadInput(argc, argv, numberOfNodes, graph, maxDepth);

	// Prepare solution variable
	vector<int> solution (pow(numberOfNodes, 2), 0);

	Timer runTimer("finish computation");

	// Queue of graphs
	deque<Problem> preparedQueue;
	deque<Problem> readyQueue;

	// Create problem for the root graph
	Problem problem(graph, 0, 1);

	// Push the root problem to the queue
	preparedQueue.push_back(problem);

	// Iterate until the cycle reaches the selected depth
	for (int depth = 0; depth < maxDepth; depth++) {

		// Iterate over all graphs in the queue
		while ( ! preparedQueue.empty()) {

			// Get the first problem in the queue
			Problem selectedProblem = preparedQueue.front();

			// Get the problem's graph
			vector<int> selectedGraph = selectedProblem.graph;

			// And pop it
			preparedQueue.pop_front();

			// Test if the selected graph is solution
			if (iterationSeq(numberOfNodes, selectedGraph, solution) == SOLUTION_FOUND) {

				// Print the solution
				printSolution(numberOfNodes, solution);

				// Exit the program
				return 0;
			}

			// Create variable for the second anchor
			int edgeY = selectedProblem.edgeY;
			
			// Iterate over all edges from given anchors
			for (int i = selectedProblem.edgeX; i < numberOfNodes; i++) {
				for (int j = edgeY; j < numberOfNodes; j++) {

					// Do not look in the bottom triangle space in the graph matrix
					if (j <= i) continue;

					// Continue if there is no edge on the given index
					if (selectedGraph[indexFor(i, j, numberOfNodes)] == 0) continue;

					// Create new graph
					vector<int> newGraph = selectedGraph;

					// Remove edges from the both triangle spaces of the matrix
					newGraph[indexFor(i, j, numberOfNodes)] = 0;
					newGraph[indexFor(j, i, numberOfNodes)] = 0;

					// Create new Problem with the new graph
					Problem problem(newGraph, i, j + 1);

					// Push the new graph to the queue
					readyQueue.push_back(problem);
				}

				// Set the new anchor
				edgeY = i + 1;
			}
		}

		// Swap the second and the first queue
		preparedQueue = readyQueue;

		// Create new empty queue
		deque<Problem> emptyQueue;

		// Swap the empty and the second queue
		swap(readyQueue, emptyQueue);
	}

	#pragma omp parallel for schedule(dynamic)

	// Iterate over all Problems in the queue
	for (int i = 0; i < (int) preparedQueue.size(); i++) {

		// Get the problem for given index
		Problem problem = preparedQueue[i];

		// Run parallel algorithm
		iteration(numberOfNodes, problem.graph, solution, problem.edgeX, problem.edgeY);
	}

	// Print the solution
	printSolution(numberOfNodes, solution);

	runTimer.end();

	return 0;
}
