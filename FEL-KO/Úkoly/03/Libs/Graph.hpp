#include <vector>

#include "Edge.hpp"

#pragma once
class Graph {
public:
    int numberOfNodes;
    vector<int> balances;
    vector<vector<Edge *> > edges;
    
    Graph(int numberOfNodes);

    int balanceForIndex(int index);
    void addEdge(Edge *edge, bool force);
    void print() const;
    int startIndex() const;
    int endIndex() const;

protected:
    void printEdges(vector<vector<Edge *> > edges) const;
};
