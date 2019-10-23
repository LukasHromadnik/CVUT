#include "header.cpp"

struct Problem {
	Problem(Graph graph, int edgeX, int edgeY);
	Graph graph;
	int edgeX;
	int edgeY;
	int numberOfNodes;

	Graph serialized();
	static Problem deserialize(Graph graph);
	int indexFor(int i, int j) const;
	int numberOfEdges() const;
	void debug() const;
	bool canRemoveEdge() const;
	bool hasEdgeBetween(int i, int j) const;
};

Problem::Problem(Graph graph, int edgeX, int edgeY) {
	this->graph = graph;
	this->edgeX = edgeX;
	this->edgeY = edgeY;
	this->numberOfNodes = (int) sqrt(graph.size());
}

Problem Problem::deserialize(Graph graph) {
	int edgeY = graph.back();
	graph.pop_back();
	int edgeX = graph.back();
	graph.pop_back();

	return Problem(graph, edgeX, edgeY);
}

Graph Problem::serialized() {
	Graph result = graph;
	result.push_back(edgeX);
	result.push_back(edgeY);

	return result;
}

int Problem::indexFor(int i, int j) const {

	// Linearization of the two-dimensional space to the single dimension
	return i * numberOfNodes + j;
}

int Problem::numberOfEdges() const {

	// Sum of the indexes in the graph
	int sum = 0;

	// Iterate over all items in graph
	for (int i = 0; i < numberOfNodes; i++) {
		for (int j = i + 1; j < numberOfNodes; j++) {

			// Increment sum with the value from given item
			sum += graph[indexFor(i, j)];
		}
	}
	
	// Return the sum
	return sum;
}

void Problem::debug() const {
	if ( ! USE_DEBUG) return;
	cout << "[" << numberOfEdges() << "] ";

	for (int i = 0; i < numberOfNodes; i++) {
		for (int j = i + 1; j < numberOfNodes; j++) {
			cout << graph[indexFor(i, j)];
		}
	}

	cout << endl;

	// for (int i = 0; i < numberOfNodes; i++) {
	// 	for (int j = 0; j < numberOfNodes; j++) {
	// 		cout << graph[indexFor(i, j)];
	// 	}
	// 	cout << endl;
	// }

	// cout << endl;
}

bool Problem::canRemoveEdge() const {

	// Check if the given graph has enough edges
	return (numberOfEdges() >= numberOfNodes);
}

bool Problem::hasEdgeBetween(int i, int j) const {
	return graph[indexFor(i, j)] == 1;
}
