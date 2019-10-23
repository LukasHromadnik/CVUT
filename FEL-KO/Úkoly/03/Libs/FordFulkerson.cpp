#include <stack>

#include "Graph.hpp"

using namespace std;

#define FRESH 0
#define OPEN 1

int * augmentingPath(Graph *graph, int start, int end) {
    stack<int> nodes;
    nodes.push(start);

    int *state = new int[graph->numberOfNodes];
    int *parent = new int[graph->numberOfNodes];
    for (int i = 0; i < graph->numberOfNodes; i++) {
        state[i] = FRESH;
        parent[i] = -1;
    }
    state[start] = OPEN;

    while ( ! nodes.empty()) {
        int node = nodes.top();
        nodes.pop();

        if (node == end) return parent;

        for (int i = 0; i < graph->numberOfNodes; i++) {
            Edge *edge = graph->edges[node][i];

            if (edge == NULL) continue;

            bool edgeCondition1 = edge->start == node && state[edge->end] == FRESH && edge->value < edge->upperBound;
            bool edgeCondition2 = edge->end == node && state[edge->start] == FRESH && edge->value > edge->lowerBound;

            if (edgeCondition1 || edgeCondition2) {
                nodes.push(i);
                state[i] = OPEN;
                parent[i] = node;
            }
        }
    }

    return NULL;
}

int FordFulkerson(Graph *graph) {
    while (true) {
        int maxFlow = INT_MAX;
        int *parent = augmentingPath(graph, graph->startIndex(), graph->endIndex());

        if (parent == NULL) break;

        vector<int> path;
        int parentIndex = graph->endIndex();
        while (parentIndex != -1) {
            path.push_back(parentIndex);
            parentIndex = parent[parentIndex];
        }

        reverse(path.begin(), path.end());

        for (int i = 1; i < (int) path.size(); i++) {
            int parent = path[i - 1];
            int current = path[i];
            Edge *edge = graph->edges[parent][current];
            if (edge->start == parent) {
                int currentMaxFlow = (edge->upperBound == INT_MAX) ? INT_MAX : edge->upperBound - edge->value;
                maxFlow = min(maxFlow, currentMaxFlow);
            } else {
                maxFlow = min(maxFlow, edge->value - edge->lowerBound);
            }
        }

        for (int i = 1; i < (int) path.size(); i++) {
            int parent = path[i - 1];
            int current = path[i];
            Edge *edge = graph->edges[parent][current];
            if (edge->start == parent) {
                edge->value += maxFlow;
            } else {
                edge->value -= maxFlow;
            }
        }
    }

    int result = 0;
    for (int i = 0; i < graph->numberOfNodes; i++) {
        Edge *edge = graph->edges[graph->startIndex()][i];
        if (edge == NULL) continue;
        result += edge->value;
    }

    return result;
}

template<typename G>
G * graphWithFeasibleFlow(G *graph) {
    Graph *fordGraph = new Graph(graph->numberOfNodes + 2);
    
    // Add circular edge to the new graph
    Edge *circularEdge = new Edge(fordGraph->endIndex() - 1, 1);
    fordGraph->addEdge(circularEdge);

    for (int i = 0; i < graph->numberOfNodes; i++) {
        for (int j = i + 1; j < graph->numberOfNodes; j++) {
            Edge *currentEdge = graph->edges[i][j];
            if (currentEdge == NULL) continue;    
            Edge *edge = new Edge(currentEdge->start + 1, currentEdge->end + 1);
            if (currentEdge->upperBound == INT_MAX) {
                edge->upperBound = currentEdge->upperBound;
            } else {
                edge->upperBound = currentEdge->upperBound - currentEdge->lowerBound;
            }
            fordGraph->addEdge(edge);
        }
    }

    for (int i = 0; i < graph->numberOfNodes; i++) {
        int balance = graph->balances[i];
        if (balance < 0) {
            Edge *edge = new Edge(i + 1, fordGraph->endIndex());
            edge->upperBound = -balance;
            fordGraph->addEdge(edge);
        } else if (balance > 0) {
            Edge *edge = new Edge(fordGraph->startIndex(), i + 1);
            edge->upperBound = balance;
            fordGraph->addEdge(edge);
        }
    }
    
    // Run Ford-Fulkerson on the graph with zero lower bounds
    int initialFlow = FordFulkerson(fordGraph);

    // Supress the unused variable warning
    (void)initialFlow;

    // Check if the instance is feasible
    int diffStart = 0;
    int diffEnd = 0;
    for (int i = 0; i < (int) fordGraph->edges.size(); i++) {
        for (int j = 0; j < (int) fordGraph->edges[i].size(); j++) {
            Edge *edge = fordGraph->edges[i][j];
            if (edge == NULL) continue;

            if (i == 0) {
                diffStart += edge->upperBound - edge->value;
            }

            if (j == fordGraph->endIndex()) {
                diffEnd += edge->upperBound - edge->value;
            }
        }
    }

    int diffFlow = min(diffStart, diffEnd);

    if (diffFlow > 0) {
        return NULL;
    }

    for (int i = 1; i < fordGraph->numberOfNodes - 1; i++) {
        for (int j = 1; j < fordGraph->numberOfNodes - 1; j++) {
            Edge *edge = fordGraph->edges[i][j];
            if (edge == NULL) continue;
            if (edge->start > edge->end) continue;

            Edge *currentEdge = graph->edges[edge->start - 1][edge->end - 1];
            currentEdge->value = edge->value + currentEdge->lowerBound;
        }
    }

    return graph;
}
