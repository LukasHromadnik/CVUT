#include <vector>
#include <iostream>
#include <iomanip>

#include "Graph.hpp"

using namespace std;

Graph::Graph(int numberOfNodes) {
    this->numberOfNodes = numberOfNodes;

    this->balances = vector<int>(numberOfNodes);

    this->edges = vector<vector<Edge *> >(numberOfNodes);
    for (int i = 0; i < numberOfNodes; i++) {
        this->edges[i] = vector<Edge *>(numberOfNodes);
    }
}

void Graph::addEdge(Edge *edge, bool force = false) {
    int start = edge->start;
    int end = edge->end;
    edges[start][end] = edge;
    if (force) {
        edges[end][start] = edge;
    }
    balances[start] -= edge->lowerBound;
    balances[end] += edge->lowerBound;
}

void Graph::printEdges(vector<vector<Edge *> > edges) const {
    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = 0; j < numberOfNodes; j++) {
            Edge *edge = edges[i][j];
            if (edge == NULL) {
                cout << setw(6) << ".";
            } else {
                cout << ((edge->start == j) ? "+" : "-");
                cout << edge->lowerBound << ",";
                cout << edge->value << ",";
                if (edge->upperBound > 1000) {
                    cout << "^";
                } else {
                    cout << edge->upperBound;
                }
            }
            cout << " ";
        }
        cout << endl;
    }
}

void Graph::print() const {
    // printEdges(edges);

    // cout << endl;
    
    // for (int i = 0; i < numberOfNodes; i++) {
    //     cout << balances[i] << " ";
    // }
    // cout << endl << endl;

    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = 0; i < numberOfNodes; j++) {
            Edge *edge = edges[i][j];
            if (edge == NULL) continue;
            edge->print();
        }
    }
}

int Graph::startIndex() const {
    return 0;
}

int Graph::endIndex() const {
    return numberOfNodes - 1;
}
