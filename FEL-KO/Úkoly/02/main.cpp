#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <climits>
#include <iomanip>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

//////////////////// EDGE ////////////////////

struct Edge {
    int lowerBound;
    int upperBound;
    int value;
    int start;
    int end;

    Edge(int start, int end);
};

Edge::Edge(int start, int end) {
    this->start = start;
    this->end = end;
    this->lowerBound = 0;
    this->upperBound = INT_MAX;
    this->value = 0;
}

//////////////////// GRAPH ////////////////////

class Graph {
public:
    int numberOfNodes;
    vector<int> balances;
    vector<vector<Edge *> > edges;
    
    Graph(int numberOfNodes);

    int balanceForIndex(int index);
    void addEdge(Edge *edge);
    void print() const;
    int startIndex() const;
    int endIndex() const;

protected:
    void printEdges(vector<vector<Edge *> > edges) const;
};

Graph::Graph(int numberOfNodes) {
    this->numberOfNodes = numberOfNodes;

    this->balances = vector<int>(numberOfNodes);

    this->edges = vector<vector<Edge *> >(numberOfNodes);
    for (int i = 0; i < numberOfNodes; i++) {
        this->edges[i] = vector<Edge *>(numberOfNodes);
    }
}

void Graph::addEdge(Edge *edge) {
    int start = edge->start;
    int end = edge->end;
    edges[start][end] = edge;
    edges[end][start] = edge;
    balances[start] -= edge->lowerBound;
    balances[end] += edge->lowerBound;
}

void Graph::printEdges(vector<vector<Edge *> > edges) const {
    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = 0; j < numberOfNodes; j++) {
            Edge *edge = edges[i][j];
            if (edge == NULL) {
                cout << setw(7) << ".";
            } else {
                cout << ((edge->start == j) ? "+" : "-");
                cout << edge->lowerBound << ",";
                cout << edge->value << ",";
                if (edge->upperBound > 1000) {
                    cout << "^ ";
                } else {
                    cout << setw(2) << edge->upperBound;
                }
            }
            cout << " ";
        }
        cout << endl;
    }
}

void Graph::print() const {
    printEdges(edges);

    cout << endl;
    
    for (int i = 0; i < numberOfNodes; i++) {
        cout << balances[i] << " ";
    }
    cout << endl << endl;
}

int Graph::startIndex() const {
    return 0;
}

int Graph::endIndex() const {
    return numberOfNodes - 1;
}

//////////////////// MY GRAPH ////////////////////

class MyGraph: public Graph {
public:
    MyGraph(int numberOfCustomers, int numberOfProducts);

    int customerIndexToIndex(int customerIndex) const;
    int productIndexToIndex(int productIndex) const;

protected:
    int numberOfCustomers;
    int numberOfProducts;
};

MyGraph::MyGraph(int numberOfCustomers, int numberOfProducts) : Graph(numberOfCustomers + numberOfProducts + 2) {
    this->numberOfCustomers = numberOfCustomers;
    this->numberOfProducts = numberOfProducts;
}

int MyGraph::customerIndexToIndex(int customerIndex) const {
    return customerIndex + 1;
}

int MyGraph::productIndexToIndex(int productIndex) const {
    return numberOfCustomers + productIndex + 1;
}

//////////////////// CUSTOMER ////////////////////

struct Customer {
    int lowerBound;
    int upperBound;
    vector<int> products;
};

//////////////////// PRODUCT ////////////////////

struct Product {
    int lowerBound;
};

//////////////////// HELPERS ////////////////////

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

int fordFulkerson(Graph *graph) {
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

//////////////////// MAIN ////////////////////

int main(int argc, const char * argv[]) {

    // Initial values
    int numberOfCustomers = 0;
    int numberOfProducts = 0;
    vector<Customer *> customers;
    vector<Product *> products;

    // Load input
    string line, word;
    int lineNumber = 0;

    // Open input file
    ifstream inFile(argv[1]);

    ////////// LOAD INPUT //////////

    // Iterate over all input lines
    while (getline(inFile, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Count word count
        int wordNumber = 0;

        // Initialize new customer
        if (lineNumber > 0 && lineNumber < numberOfCustomers + 1) {
            customers.push_back(new Customer());
        }

        // Iterate over all "token"
        while (ss >> word) {

            // Convert input to the integer
            int input = stoi(word);

            if (lineNumber == 0) {
                if (wordNumber == 0) {
                    numberOfCustomers = input;
                } else {
                    numberOfProducts = input;
                }
            } else if (lineNumber < numberOfCustomers + 1) {
                int customerIndex = lineNumber - 1;
                Customer *customer = customers[customerIndex];
                if (wordNumber == 0) {
                    customer->lowerBound = input;
                } else if (wordNumber == 1) {
                    customer->upperBound = input;
                } else {
                    customer->products.push_back(input - 1);
                }
            } else {
                Product *product = new Product();
                product->lowerBound = input;
                products.push_back(product);
            }

            // Increase word number
            wordNumber++;
        }

        // Increase line number
        lineNumber++;
    }

    ////////// CREATE GRAPH //////////

    MyGraph *graph = new MyGraph(numberOfCustomers, numberOfProducts);

    // Create edges from the start to every customer
    for (int i = 0; i < numberOfCustomers; i++) {
        Edge *edge = new Edge(graph->startIndex(), graph->customerIndexToIndex(i));
        edge->lowerBound = customers[i]->lowerBound;
        edge->upperBound = customers[i]->upperBound;
        graph->addEdge(edge);
    }

    // Create relationships between customer and his products
    for (int i = 0; i < numberOfCustomers; i++) {
        for (int productIndex : customers[i]->products) {
            Edge *edge = new Edge(graph->customerIndexToIndex(i), graph->productIndexToIndex(productIndex));
            edge->upperBound = 1;
            graph->addEdge(edge);
        }
    }

    // Create edges from every product to the end
    for (int i = 0; i < numberOfProducts; i++) {
        Edge *edge = new Edge(graph->productIndexToIndex(i), graph->endIndex());
        edge->lowerBound = products[i]->lowerBound;
        graph->addEdge(edge);
    }

    ////////// FORD-FULKERSON //////////

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
    int initialFlow = fordFulkerson(fordGraph);

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

    ofstream outFile(argv[2]);

    if (diffFlow > 0) {
        outFile << "-1";
        outFile.close();
        cout << "INFEASIBLE (" << diffFlow << ")" << endl;
        return 0;
    }

    fordGraph->print();

    // for (int i = 0; i < graph->numberOfNodes; i++) {
    //     for (int j = 0; j < graph->numberOfNodes; j++) {
    //         Edge *edge = graph->edges[i][j];
    //         if (edge == NULL) continue;

    //         cout << edge->start << " " << edge->end << endl;

    //     }
    // }

    // cout << endl;

    for (int i = 1; i < fordGraph->numberOfNodes - 1; i++) {
        for (int j = 1; j < fordGraph->numberOfNodes - 1; j++) {
            Edge *edge = fordGraph->edges[i][j];
            if (edge == NULL) continue;
            if (edge->start > edge->end) continue;

            Edge *currentEdge = graph->edges[edge->start - 1][edge->end - 1];
            currentEdge->value = edge->value + currentEdge->lowerBound;

            // cout << edge->start - 1 << " " << edge->end - 1 << endl;

        }
    }

    graph->print();

    // return 0;


    int maxFlow = fordFulkerson(graph);
    (void)maxFlow;

    graph->print();

    for (int i = 0; i < numberOfCustomers; i++) {
        int customer = graph->customerIndexToIndex(i);
        bool found = false;
        for (int j = 0; j < numberOfProducts; j++) {
            int product = graph->productIndexToIndex(j);
            Edge *edge = graph->edges[customer][product];
            if (edge == NULL || edge->value == 0) continue;
            if (found) {
                outFile << " ";
            }
            outFile << j + 1;
            if ( ! found) {
                found = true;
            }
        }

        if (i + 1 != numberOfCustomers) {
            outFile << endl;
        }
    }

    outFile.close();

    return 0;
}
