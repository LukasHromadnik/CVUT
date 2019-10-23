#include "Problem.cpp"
#include "Timer.cpp"

int isComplete(Problem problem) {

	// Initialize the queue of nodes
	queue<int> queue;

	// Initialize colors for each node
	int *colors = new int[problem.numberOfNodes];

	// Set all colors to the undefined
	for (int i = 0; i < problem.numberOfNodes; i++) {
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
		for (int i = 0; i < problem.numberOfNodes; i++) {

			int nodeIndex = problem.indexFor(node, i);

			// Check if the selected node with given node have edge
			if (problem.graph[nodeIndex] == 1 && colors[i] == -1) {

				// Set opposite color for the given node
				colors[i] = 0;

				// Push the given node in to the queue
				queue.push(i);
			}
		}
	}

	// Check if the graph is complete
	for (int i = 0; i < problem.numberOfNodes; i++) {

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
	return GRAPH_COMPLETE;
}

int isBipartite(Problem problem) {

	// Initialize the queue of nodes
	queue<int> queue;

	// Initialize colors for each node
	int *colors = new int[problem.numberOfNodes];

	// Set all colors to the undefined
	for (int i = 0; i < problem.numberOfNodes; i++) {
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
		for (int i = 0; i < problem.numberOfNodes; i++) {

			int nodeIndex = problem.indexFor(node, i);

			// Check if the selected node with given node have edge
			if (problem.graph[nodeIndex] == 1 && colors[i] == -1) {

				// Set opposite color for the given node
				colors[i] = 1 - colors[node];

				// Push the given node in to the queue
				queue.push(i);
			}

			// If the selected node and the given node have edge and same colors
			// the graph is not bipartite
			else if (problem.graph[nodeIndex] == 1 && colors[node] == colors[i]) {

				// Empty allocated memory
				delete[] colors;

				// Return for not bipartite graph
				return GRAPH_NOT_BIPARTITE;
			}
		}
	}

	// Empty allocated memory
	delete[] colors;

	// Return for complete and bipartite graph
	return GRAPH_BIPARTITE;
}

void printSolution(Problem problem) {

	// Print the founded solution
	problem.debug();

	// Print the number of edges of the solution
	cout << problem.numberOfEdges() << endl;
}

int iterationSeq(Problem problem, Problem &solution, int useDebug = 0) {

	// Create helpers for edge counts
	int numberOfEdgesOfData = problem.numberOfEdges();
	int numberOfEdgesOfSolution = solution.numberOfEdges();

	if (useDebug) {
		cout << numberOfEdgesOfData << " " << numberOfEdgesOfSolution << endl;
	}

	// Check if the given graph can have more edges than the current solution
	if (numberOfEdgesOfData <= numberOfEdgesOfSolution) {
		if (useDebug) {
		cout << "less edges" << endl;
	}
		return SOLUTION_NOT_FOUND;
	}

	if (isComplete(problem) == GRAPH_NOT_COMPLETE) {
		if (useDebug) {
		cout << "not complete" << endl;
	}
		return SOLUTION_NOT_FOUND;
	}

	// Test if the graph is bipartite and complete)
	if (isBipartite(problem) == GRAPH_BIPARTITE) {
		if (useDebug) {
		cout << "is bipartite" << endl;
	}

		#pragma omp critical
		{
			// Check the solution again and then set the new solution
			if (numberOfEdgesOfData > solution.numberOfEdges()) {

				// Set the new solution
				solution = problem;

				// cout << "[CRITICAL] " << numberOfEdgesOfData << endl;
			}
		}

		// Return value for new solution
		return SOLUTION_FOUND;
	}
	if (useDebug) {
	cout << "solution not found" << endl;
}

	// Graph did not update the solution
	return SOLUTION_NOT_FOUND;
}

void iteration03(Problem problem, Problem &solution, int useDebug = 0) {

	if (useDebug) {
		problem.debug();
	}

	// Create helpers for edge counts
	int numberOfEdgesOfData = problem.numberOfEdges();
	int numberOfEdgesOfSolution = solution.numberOfEdges();

	// Check if the given graph can have more edges than the current solution
	if (numberOfEdgesOfData <= numberOfEdgesOfSolution) return;

	if (isComplete(problem) == GRAPH_NOT_COMPLETE) return;

	// Graph is bipartite
	if (isBipartite(problem) == GRAPH_BIPARTITE) {

		#pragma omp critical
		{
			// Check the solution again and then set the new solution
			if (numberOfEdgesOfData > solution.numberOfEdges()) {

				// Set the new solution
				solution = problem;
			}
		}

		// No need to continue processing
		return;
	}

	// Check if children can get better solution
	if (numberOfEdgesOfData - 1 <= numberOfEdgesOfSolution) return;

	// Check if the given graph has enough edges
	if ( ! problem.canRemoveEdge()) return;

	int edgeY = problem.edgeY;

	// Iterate over all nodes
	// Set the first node to the x edge
	for (int i = problem.edgeX; i < problem.numberOfNodes; i++) {

		// Set the second node the y edge
		for (int j = edgeY; j < problem.numberOfNodes; j++) {

			// Check if the index is in the upper right side from the diagonal
			if (j <= i) continue;

			// Check if given two nodes having a edge
			if ( ! problem.hasEdgeBetween(i, j)) continue;

			// Copy current graph
			Graph newGraph = problem.graph;

			// Remove edge from the new graph
			newGraph[problem.indexFor(i, j)] = 0;
			newGraph[problem.indexFor(j, i)] = 0;

			// Create new problem
			Problem newProblem(newGraph, i, j + 1);
			
			// Run the iteration with new edges and graph
			iteration03(newProblem, solution);
		}

		// Reset the y edge for the next loop
		edgeY = i + 1;
	}
}

void loadInput(int numberOfParameters, char **parameters, int &numberOfNodes, Graph &graph, int &maxDepthProcesses, int &maxDepthThreads) {

	// Check parameters
	if (numberOfParameters != 4) {
		cout << "--------------------------" << endl;
		cout << "WRONG NUMBER OF PARAMETERS" << endl;
		cout << "Usage: binary input-file max-depth-for-processes max-depth-for-threads" << endl;
		cout << "--------------------------" << endl;
		exit(1);
	}

	// Load max depth for processes from cmd
	maxDepthProcesses = atoi(parameters[2]);

	// Load max depth for threads from cmd
	maxDepthThreads = atoi(parameters[3]);

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

deque<Problem> generateBFS(Problem problem, Problem &solution, int maxDepth, int useDebug = 0) {

	if (useDebug) {
		problem.debug();
	}
	
	// Queue of graphs
	deque<Problem> preparedQueue;

	// Push the root problem to the queue
	preparedQueue.push_back(problem);

	// Iterate until the cycle reaches the selected depth
	for (int depth = 0; depth < maxDepth; depth++) {

		deque<Problem> readyQueue;

		// Iterate over all graphs in the queue
		while ( ! preparedQueue.empty()) {

			// Get the first problem in the queue
			Problem selectedProblem = preparedQueue.front();

			// And pop it
			preparedQueue.pop_front();

			// Test if the selected graph is solution
			if (iterationSeq(selectedProblem, solution, useDebug) == SOLUTION_FOUND) {

				// Exit the program
				return preparedQueue;
			}

			// Create variable for the second anchor
			int edgeY = selectedProblem.edgeY;
			
			// Iterate over all edges from given anchors
			for (int i = selectedProblem.edgeX; i < selectedProblem.numberOfNodes; i++) {
				for (int j = edgeY; j < selectedProblem.numberOfNodes; j++) {

					// Do not look in the bottom triangle space in the graph matrix
					if (j <= i) continue;

					// Continue if there is no edge on the given index
					if (selectedProblem.graph[selectedProblem.indexFor(i, j)] == 0) continue;

					// Create new graph
					Graph newGraph = selectedProblem.graph;

					// Remove edges from the both triangle spaces of the matrix
					newGraph[selectedProblem.indexFor(i, j)] = 0;
					newGraph[selectedProblem.indexFor(j, i)] = 0;

					// Create new Problem with the new graph
					Problem newProblem(newGraph, i, j + 1);

					// Push the new graph to the queue
					readyQueue.push_back(newProblem);
				}

				// Set the new anchor
				edgeY = i + 1;
			}
		}

		// Swap the second and the first queue
		preparedQueue = readyQueue;
	}

	// Return graphs in the queue
	return preparedQueue;
}
