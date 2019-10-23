#include <set>
#include <vector>
#include <climits>

#include "Graph.hpp"

using namespace std;

vector<pair<int, int> > BellmanFord(Graph *graph) {
    int numberOfNodes = graph->numberOfNodes;

    vector<int> distances(numberOfNodes, INT_MAX);
    vector<int> predecessors(numberOfNodes, -1);

    distances[0] = 0;
    bool useDebug = false;

    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = 0; j < numberOfNodes; j++) {
            for (Edge *edge : graph->edges[j]) {
                if (edge == NULL) continue;

                int u = edge->start;
                int v = edge->end;
                int cost = edge->cost;

                if (distances[u] != INT_MAX && distances[u] + cost < distances[v]) {
                    distances[v] = distances[u] + cost;
                    predecessors[v] = u;
                }
            }
        }
    }

    if (useDebug) {
        for (int i = 0; i < numberOfNodes; i++) {
            cout << "[PRED] " << i << " " << predecessors[i] << endl;
        }
        cout << endl;
        for (int i = 0; i < numberOfNodes; i++) {
            cout << "[DIST] " << i << " " << distances[i] << endl;
        }
    }

    for (int i = 0; i < numberOfNodes; i++) {
        for (Edge *edge : graph->edges[i]) {
            if (edge == NULL) continue;

            int u = edge->start;
            int v = edge->end;
            int cost = edge->cost;
    
            if (distances[u] != INT_MAX && distances[u] + cost < distances[v]) {
                int firstNode = u;
                int parent = firstNode;
                
                vector<pair<int, int> > negativeCycle;
                if (useDebug) {
                    cout << "Záporný cyklus" << endl;
                    cout << u << " " << v << endl;
                    graph->print();
                }

                set<int> nodes;

                while (nodes.find(parent) == nodes.end()) {
                    nodes.insert(parent);
                    parent = predecessors[parent];
                }

                firstNode = parent;
                
                do {
                    negativeCycle.push_back(pair<int, int>(predecessors[parent], parent));
                    parent = predecessors[parent];
                } while (parent != firstNode);
                
                return negativeCycle;
            }
        }
    }

    return vector<pair<int, int> >();
}
