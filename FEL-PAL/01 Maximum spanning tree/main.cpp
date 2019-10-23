#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <limits.h>
#include <algorithm>

#include "Node.cpp"
#include "Edge.cpp"
#include "Graph.cpp"
#include "Span.cpp"

using namespace std;

class Value {
public:
    Value(int value, int count);
    int m_Value;
    int m_Count;
};

Value::Value(int value, int count) {
    m_Value = value;
    m_Count = count;
}

struct ValueGreater {
    bool operator() (Value * a, Value * b) {
        return a->m_Count <= b->m_Count;
    }
};

Span * createSpan(vector<Edge *> edges, int maxEdgeValue) {
    Span *span = new Span(maxEdgeValue);

    for (Edge *edge : edges) {
        Node *firstParent = edge->m_Nodes.first->parent();
        Node *secondParent = edge->m_Nodes.second->parent();

        // Check if the edge can create cycle
        if (firstParent->m_Index == secondParent->m_Index) continue;

        Node *biggerParent = (firstParent->m_Rank >= secondParent->m_Rank) ? firstParent : secondParent;
        Node *smallerParent = (firstParent->m_Rank >= secondParent->m_Rank) ? secondParent : firstParent;
        
        biggerParent->m_Rank = max(biggerParent->m_Rank, smallerParent->m_Rank + 1);
        smallerParent->m_Parent = biggerParent;
        
        span->addEdge(edge);
    }

    return span;
}

int main(int argc, const char * argv[]) {   
    string line, word;
    int lineNumber = 0;
    Graph *graph;
    int maxEdgeValue = 0;
    long long int minSpan = -1;
    int maxCountOfSimilarEdges = 0;

    // Iterate over all input lines
    while (getline(cin, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Parse input line
        vector<int> tokens;

        // Split input line by space and convert the split result to the integer value
        // Then push it to the `tokens` vector
        while (ss >> word) {
            tokens.push_back(stoi(word));
        }

        // Initialize the graph
        if (lineNumber == 0) {
            int numberOfNodes = tokens[0];
            int numberOfEdges = tokens[1];

            // Create graph dynamically
            graph = new Graph(numberOfNodes, numberOfEdges);
        }

        // Add new edge to the graph
        else {
            int firstNodeIndex = tokens[0] - 1;
            int secondNodeIndex = tokens[1] - 1;
            int edgeValue = tokens[2];
            graph->addEdge(firstNodeIndex, secondNodeIndex, edgeValue);

            maxEdgeValue = max(maxEdgeValue, edgeValue);
        }

        // Increase line number
        lineNumber++;
    }

    vector<Edge *> *sortedEdges = new vector<Edge *>[maxEdgeValue];
    for (int i = 0; i < maxEdgeValue; i++) {
        sortedEdges[i] = vector<Edge *>();
    }

    // Generate sorted array of edges
    for (Edge *edge : graph->m_Edges) {
        sortedEdges[edge->m_Value - 1].push_back(edge);
    }

    vector<Value *> values;
    for (int i = 0; i < maxEdgeValue; i++) {
        int count = sortedEdges[i].size();
        if (count == 0) continue;
        Value *newValue = new Value(i, count);
        values.push_back(newValue);
    }

    make_heap(values.begin(), values.end(), ValueGreater());

    int maxValue = INT_MAX;
    int maxCount = INT_MAX;
    int currentMaxValue = -1;
    int currentMaxCount = 0;

    // Generate edges according to the priorities
    // for (int i = 0; i < maxEdgeValue; i++) {
    //     for (int j = 0; j < maxEdgeValue; j++) {
    //         int count = sortedEdges[j].size();

    //         // if (maxCountOfSimilarEdges != 0 && count < maxCountOfSimilarEdges) continue;

    //         int value = j;

    //         if (value > currentMaxValue && count >= currentMaxCount && value < maxValue && count <= maxCount) {
    //             currentMaxCount = count;
    //             currentMaxValue = value;
    //         }
    //     }
    Value *selected = values[0];
    values.erase(values.begin());
    make_heap(values.begin(), values.end(), ValueGreater());

    while (true) {
        currentMaxCount = selected->m_Count;
        currentMaxValue = selected->m_Value;

        if (currentMaxCount < maxCount && maxCount != INT_MAX) {
            cout << minSpan << endl;
            return 0;
        }

        // if (currentMaxCount == 0) continue;

        vector<Edge *> edges;

        for (Edge *edge : sortedEdges[currentMaxValue]) {
            edges.push_back(edge);
        }

        for (int j = 0; j < maxEdgeValue; j++) {
            if (j == currentMaxValue) continue;
            for (Edge *edge : sortedEdges[j]) {
                edges.push_back(edge);
            }
        }

        Span *span = createSpan(edges, maxEdgeValue);
        if (span->m_MaxCountOfSimilarEdges > maxCountOfSimilarEdges) {
            minSpan = span->m_Value;
            maxCountOfSimilarEdges = span->m_MaxCountOfSimilarEdges;
        } else if (span->m_MaxCountOfSimilarEdges == maxCountOfSimilarEdges) {
            if (minSpan == -1) {
                minSpan = span->m_Value;
            } else {
                minSpan = min(minSpan, span->m_Value);
            }
        }

        for (Node *node : graph->m_Nodes) {
            node->m_Parent = node;
        }

        delete span;

        maxValue = currentMaxValue;
        maxCount = currentMaxCount;
        currentMaxCount = 0;
        currentMaxValue = -1;

        selected = values[0];
        values.erase(values.begin());
        make_heap(values.begin(), values.end(), ValueGreater());
    }

    cout << minSpan << endl;

    delete[] sortedEdges;

    return 0;
}
