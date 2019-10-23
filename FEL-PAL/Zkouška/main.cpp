#include <iostream>
#include <string>
#include <iomanip>
#include <numeric>
#include <limits.h>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

#define OPEN 0
#define VISITED 1
#define CLOSED 2

class Span {
public:
    Span(int *span, int numberOfNodes, int value);
    int *span;
    int value;
};

Span::Span(int *span, int numberOfNodes, int value) {
    this->span = new int[numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++) {
        this->span[i] = span[i];
    }
    this->value = value;
}

class Node;

class Edge {
public:
    Edge(Node *firstNode, Node *secondNode, int value);
    Node *firstNode;
    Node *secondNode;
    int value;
};

Edge::Edge(Node *firstNode, Node *secondNode, int value) {
    this->firstNode = firstNode;
    this->secondNode = secondNode;
    this->value = value;
}

class MNode {
public:
    MNode(int identifier, MNode *parent);
    int identifier;
    MNode* parent;
};

MNode::MNode(int identifier, MNode *parent) {
    this->identifier = identifier;
    this->parent = parent;
}

class Node {
public:
    Node(int identifier);
    vector<Edge *> edges;
    int identifier;
    int debugIdentifier();
    int state;
    Node *parent;
};

Node::Node(int identifier) {
    this->identifier = identifier;
    this->state = OPEN;
    this->parent = NULL;
}

int Node::debugIdentifier() {
    return identifier + 1;
}

int main(int argc, const char * argv[]) {
    // Helpers for input
    string line, word;
    int lineNumber = 0;

    vector<Node *> nodes;
    // vector<Edge *> edges;
    int numberOfNodes = 0;
    int numberOfEdges = 0;
    int startNodeIndex = 0;
    int endNodeIndex = 0;
    int maxEdgeValue = 0;

    int **weights = NULL;

    // Iterate over all input lines
    while (getline(cin, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Parse input line
        vector<string> tokens;

        // Split input line by space and convert the split result to the integer value
        // Then push it to the `tokens` vector
        while (ss >> word) {
            tokens.push_back(word);
        }

        if (lineNumber == 0) {
            numberOfNodes = stoi(tokens[0]);
            numberOfEdges = stoi(tokens[1]);
            int node1 = stoi(tokens[2]) - 1;
            int node2 = stoi(tokens[3]) - 1;
            startNodeIndex = min(node1, node2);
            endNodeIndex = max(node1, node2);

            weights = new int*[numberOfNodes];
            for (int i = 0; i < numberOfNodes; i++) {
                nodes.push_back(new Node(i));

                weights[i] = new int [numberOfNodes];
                for (int j = 0; j < numberOfNodes; j++) {
                    weights[i][j] = 0;
                }
            }
        } else {
            int firstNodeIndex = stoi(tokens[0]) - 1;
            int secondNodeIndex = stoi(tokens[1]) - 1;
            int value = stoi(tokens[2]);

            Node *firstNode = nodes[firstNodeIndex];
            Node *secondNode = nodes[secondNodeIndex];

            Edge *firstEdge = new Edge(firstNode, secondNode, value);
            nodes[firstNodeIndex]->edges.push_back(firstEdge);
            
            Edge *secondEdge = new Edge(secondNode, firstNode, value);
            nodes[secondNodeIndex]->edges.push_back(secondEdge);

            weights[firstNodeIndex][secondNodeIndex] = value;
            weights[secondNodeIndex][firstNodeIndex] = value;

            if (value > maxEdgeValue) {
                maxEdgeValue = value;
            }
            // edges.push_back(firstEdge);
        }

        // Increase line number
        lineNumber++;
    }

    queue<MNode *> nodesQueue;
    queue<MNode *> nextNodesQueue;
    int depth = 0;
    bool found = false;
    
    nodesQueue.push(new MNode(startNodeIndex, NULL));
    vector<MNode *> solution;
    
    while (true) {
        MNode *mnode = nodesQueue.front();
        nodesQueue.pop();
        
        Node *node = nodes[mnode->identifier];
        node->state = VISITED;

        for (int i = 0; i < (int) node->edges.size(); i++) {
            Edge *edge = node->edges[i];
            Node *brother = edge->secondNode;

            if (brother->identifier == endNodeIndex) {
                found = true;
                solution.push_back(new MNode(brother->identifier, mnode));
                break;
            }

            if (brother->state != OPEN) continue;
            nextNodesQueue.push(new MNode(brother->identifier, mnode));
        }

        if (nodesQueue.empty()) {
            depth++;
            if (found) break;
            nodesQueue = nextNodesQueue;
            while ( ! nextNodesQueue.empty()) {
                nextNodesQueue.pop();
            }
        }
    }

    int minSpan = INT_MAX;
    int minSpanBegin = 0;
    int *span = new int[numberOfNodes];
    int *nextNodes = new int[numberOfNodes];

    // Generate sorted array of edges
    // for (Edge *edge : edges) {
    //     sortedEdges[edge->value - 1].push_back(edge);
    // }

    // MNode *node = solution[0];

    vector<Span *> spans;

    for (MNode *node : solution) {
        int spanValue = 0;
        MNode *parent = node;

        for (int i = 0; i < numberOfNodes; i++) {
            span[i] = 0;
        }

        while (parent != NULL) {
            // cout << parent->identifier + 1 << " ";
            span[parent->identifier] = 1;
            if (parent->parent != NULL) {
                spanValue += weights[parent->identifier][parent->parent->identifier];
            }
            parent = parent->parent;
        }

        spans.push_back(new Span(span, numberOfNodes, spanValue));

        // for (int i = 0; i < numberOfNodes; i++) {
        //     cout << span[i] << " ";
        // }
        // cout << "[" << spanValue << "]" << endl;
    }

    sort(spans.begin(), spans.end(), [](Span *a, Span *b) {
        return a->value < b->value;
    });

    // for (int i = 0; i < spans.size(); i++) {
    //     cout << "[" << spans[i]->value << "]" << endl;
    // }
    
    // for (int i = 0; i < (int) spans.size(); i++) {
    for (int i = 0; i < 1; i++) {
        Span *spanObject = spans[i];
    
    // for (Span *spanObject : spans) {
        int *span = spanObject->span;
        // for (int i = 0; i < numberOfNodes; i++) {
        //     cout << spanObject->span[i] << " ";
        // }
        // cout << "[" << spanObject->value << "]" << endl;

    // for (MNode *node : solution) {
        vector<Edge *> *sortedEdges = new vector<Edge *>[maxEdgeValue];
        for (int i = 0; i < maxEdgeValue; i++) {
            sortedEdges[i] = vector<Edge *>();
        }

        int spanValue = spanObject->value;
    //     for (int i = 0; i < numberOfNodes; i++) {
    //         span[i] = 0;
    //         nextNodes[i] = 0;
    //     }

        int sum = depth + 1;
    //     MNode *parent = node;

        // while (parent != NULL) {
        //     // cout << parent->identifier + 1 << " ";
        //     span[parent->identifier] = 1;
        //     if (parent->parent != NULL) {
        //         spanValue += weights[parent->identifier][parent->parent->identifier];
        //     }
        //     parent = parent->parent;
        // }
        // // cout << endl;

        for (int i = 0; i < numberOfNodes; i++) {
            // cout << span[i] << " ";
            if (span[i] == 0) continue;
            Node *node = nodes[i];
            for (Edge *edge : node->edges) {
                int nextNodeIndex = edge->secondNode->identifier;
                if (span[nextNodeIndex] == 1) continue;
                nextNodes[nextNodeIndex] = 1;
                sortedEdges[edge->value - 1].push_back(edge);
            }
        }
        // cout << endl;

        // return 0;

        
        
        while (sum < numberOfNodes) {
            // cout << "tu1" << endl;
            if (spanValue >= minSpan) break;

            // for (int i = 0; i < numberOfNodes; i++) {
            //     cout << nextNodes[i] << " ";
            // }
            // cout << endl << endl;

            int minWeight = INT_MAX;
            int minNode = -1;

            /*
            for (int i = 0; i < numberOfNodes; i++) {
                if (nextNodes[i] == 0) continue;

                Node *node = nodes[i];
                for (Edge *edge : node->edges) {
                    int parentIndex = edge->secondNode->identifier;
                    if (span[parentIndex] == 0) continue;
                    if (minWeight > edge->value) {
                        minWeight = edge->value;
                        minNode = i;
                        // cout << "[MIN] " << minNode + 1 << " " << minWeight << endl;
                    }
                }
            }
            */

            for (int i = 0; i < maxEdgeValue; i++) {
                vector<Edge *> edgesWithValue = sortedEdges[i];
                if (edgesWithValue.empty()) continue;
                for (int j = 0; j < (int) edgesWithValue.size(); j++) {
                    Edge *edge = edgesWithValue[j];
                    if (span[edge->secondNode->identifier] == 1) continue;
                    if (minWeight > edge->value) {
                        minWeight = edge->value;
                        minNode = edge->secondNode->identifier;
                        // cout << "[MIN] " << minNode + 1 << " " << minWeight << endl;
                        edgesWithValue.erase(edgesWithValue.begin() + j);
                        break;
                    }
                }
            }


            for (int i = 0; i < maxEdgeValue; i++) {
                for (int j = 0; j < (int) sortedEdges[i].size(); j++) {
                    int nodeIdentifier = sortedEdges[i][j]->secondNode->identifier;
                    if (nodeIdentifier == minNode || span[nodeIdentifier] == 1) {
                        sortedEdges[i].erase(sortedEdges[i].begin() + j);
                    }
                }
            }

            // cout << "begin " << minNode << endl;
            for (Edge *edge : nodes[minNode]->edges) {
                int nextNodeIndex = edge->secondNode->identifier;
                if (span[nextNodeIndex] == 1) continue;
                nextNodes[nextNodeIndex] = 1;
                sortedEdges[edge->value - 1].push_back(edge);
            }
            // cout << "end" << endl;

            span[minNode] = 1;
            nextNodes[minNode] = 0;
            spanValue += minWeight;
            sum++;
            // cout << "tu2" << endl;
        }

        if (spanValue < minSpan) {
            minSpan = spanValue;
            minSpanBegin = spanObject->value;
        }

        // cout << "[" << spanValue << "]" << endl;
    }

    cout << depth << " " << minSpan << endl;
    // cout << minSpanBegin << endl;

    return 0;
}
