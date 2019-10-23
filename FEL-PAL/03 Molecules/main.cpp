#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <algorithm>
#include <list>

using namespace std;

// ------------------------------------------------------------------------------------------------

bool compareIdentifiers(string a, string b) {
    int max = min(a.length(), b.length());
    for (int i = 0; i < max; i++) {
        string firstA = string(1, a[i]);
        string firstB = string(1, b[i]);
        int comparison = firstA.compare(firstB);
        if (comparison == 0) continue;
        return comparison < 0;
    }

    return false;
}

int debugIndex(int index) {
    return index + 1;
}

// ------------------------------------------------------------------------------------------------

class Cycle {
public:
    int node1;
    int node2;
    int node3;
    Cycle(int node1, int node2, int node3);
};

Cycle::Cycle(int node1, int node2, int node3) {
    this->node1 = node1;
    this->node2 = node2;
    this->node3 = node3;
}

// ------------------------------------------------------------------------------------------------

class Graph {
private:
    int numberOfNodes;
    int * cycleNodes;
    string * identifiers;
    void removeCycle(Cycle *cycle);
    int * findLeafs() const;
public:
    int ** nodes;
    vector<list<int> > adjancency;
    Graph(int numberOfNodes);
    void addEdge(int v, int w);
    void condensate();
    string identifier();
    void debug() const;
};
 
Graph::Graph(int numberOfNodes) {
    this->numberOfNodes = numberOfNodes;

    nodes = new int*[numberOfNodes];
    cycleNodes = new int[numberOfNodes];
    identifiers = new string[numberOfNodes];
    adjancency = vector<list<int>>(numberOfNodes);
    for (int i = 0; i < numberOfNodes; i++) {
        nodes[i] = new int[numberOfNodes];
        for (int j = 0; j < numberOfNodes; j++) {
            nodes[i][j] = 0;
        }

        identifiers[i] = "";
        cycleNodes[i] = 0;
    }
}
 
void Graph::addEdge(int v, int w) {
    int firstNode = v;
    int secondNode = w;
    nodes[firstNode][secondNode] = 1;
    nodes[secondNode][firstNode] = 1;

    if (find(adjancency[firstNode].begin(), adjancency[firstNode].end(), secondNode) == adjancency[firstNode].end()) {
        adjancency[firstNode].push_back(secondNode);
    }

    if (find(adjancency[secondNode].begin(), adjancency[secondNode].end(), firstNode) == adjancency[secondNode].end()) {
        adjancency[secondNode].push_back(firstNode);
    }
    // adjancency[secondNode].push_back(firstNode);
}

void Graph::removeCycle(Cycle *cycle) {
    set<int> children;

    for (int child : adjancency[cycle->node1]) {
        children.insert(child);
    }

    for (int child : adjancency[cycle->node2]) {
        children.insert(child);
    }

    for (int child : adjancency[cycle->node3]) {
        children.insert(child);
    }

    for (int i : children) {
        nodes[cycle->node1][i] = 0;
        nodes[i][cycle->node1] = 0;
        nodes[cycle->node2][i] = 0;
        nodes[i][cycle->node2] = 0;
        nodes[cycle->node3][i] = 0;
        nodes[i][cycle->node3] = 0;
        adjancency[i].remove(cycle->node1);
        adjancency[i].remove(cycle->node2);
        adjancency[i].remove(cycle->node3);

        if (i == cycle->node1 || i == cycle->node2 || i == cycle->node3) continue;

        addEdge(i, cycle->node1);
    }

    adjancency[cycle->node2] = list<int>();
    adjancency[cycle->node3] = list<int>();

    cycleNodes[cycle->node1] = 1;

    // cout << debugIndex(cycle->node1) << " " << debugIndex(cycle->node2) << " " << debugIndex(cycle->node3) << " " << endl;
}

void Graph::debug() const {
    set<int> _nodes;

    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = i + 1; j < numberOfNodes; j++) {
            // cout << nodes[i][j];
            if (nodes[i][j] == 1) {
                _nodes.insert(i);
                _nodes.insert(j);
            }
        }
    }

    // for (auto it : _nodes) {
    //     cout << it + 1 << " ";
    // }
    // cout << endl;
}

void Graph::condensate() {
    int *visited = new int[numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++) {
        visited[i] = 0;
    }

    queue<int> q;
    q.push(0);
    vector<Cycle *> cycles;

    while ( ! q.empty()) {
        int node = q.front();
        q.pop();
        visited[node] = 2;
        Cycle *cycle = NULL;
        bool foundNotClosedNode = false;

        for (int child : adjancency[node]) {
            if (visited[child] == 0) {
                q.push(child);
                visited[child] = 1;
            } else if (visited[child] == 1 || visited[child] == 2) {
                if (visited[child] == 1) {
                    foundNotClosedNode = true;
                }
                if (cycle != NULL) {
                    cycle->node3 = child;
                } else {
                    cycle = new Cycle(node, child, -1);
                }
            }
        }

        if (cycle != NULL && cycle->node3 != -1 && foundNotClosedNode) {
            cycles.push_back(cycle);
        }
    }

    for (Cycle *cycle : cycles) {
        removeCycle(cycle);
    }
}

int * Graph::findLeafs() const {
    int * leafs = new int[numberOfNodes];
    
    for (int i = 0; i < numberOfNodes; i++) {
        leafs[i] = (adjancency[i].size() == 1) ? 1 : 0;
    }

    return leafs;
}

string Graph::identifier() {
    int * leafs = findLeafs();
    debug();
    while (true) {
        for (int i = 0; i < numberOfNodes; i++) {
            // cout << "[NODE] " << debugIndex(i) << endl;
            if (adjancency[i].size() == 0) continue;
            vector<string> nodeIdentifiers;
            nodeIdentifiers.push_back(identifiers[i]);

            vector<int> toRemove;

            // for (int child : adjancency[i]) {
            //     cout << debugIndex(child) << " ";
            // }
            // cout << endl;

            for (int j : adjancency[i]) {
                if (leafs[j] == 1) {
                    // cout << "[LEAF] " << debugIndex(j) << endl;
                    if (leafs[i] == 1) {
                        nodeIdentifiers.push_back(identifiers[j]);
                    } else if (cycleNodes[j] == 1) {
                        nodeIdentifiers.push_back("2" + identifiers[j] + "1");
                    } else {
                        nodeIdentifiers.push_back("0" + identifiers[j] + "1");
                    }
                    identifiers[j] = "";
                    nodes[i][j] = 0;
                    nodes[j][i] = 0;
                    adjancency[j] = list<int>();
                    toRemove.push_back(j);
                }
            }

            for (int node : toRemove) {
                adjancency[i].remove(node);
            }

            if (nodeIdentifiers.size() == 1) {
                identifiers[i] = nodeIdentifiers[0];
            } else {
                sort(nodeIdentifiers.begin(), nodeIdentifiers.end(), compareIdentifiers);
                string nodeIdentifier = "";
                for (string identifier : nodeIdentifiers) {
                    // cout << identifier << endl;
                    nodeIdentifier += identifier;
                }
                identifiers[i] = nodeIdentifier;
            }
        }

        int foundSolution = 0;
        int lastIndex = -1;
        for (int i = 0; i < numberOfNodes; i++) {
            // cout << "[" << debugIndex(i) << "] " << identifiers[i] << endl;
            if (identifiers[i].length() > 1) {
                foundSolution += 1;
                lastIndex = i;
            }
        }

        // cout << endl;
        
        delete[] leafs;

        if (foundSolution == 1) {
            return identifiers[lastIndex];
        }

        leafs = findLeafs();
    }

    return "";
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

int main(int argc, const char * argv[]) {   
    string line, word;
    int lineNumber = 0;

    int numberOfAtoms = 0;
    int numberOfBonds = 0;
    int numberOfMolecules = 0;

    int moleculeNumber = 0;
    int bondNumber = 0;

    vector<Graph *> molecules;

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

        if (lineNumber == 0) {
            numberOfAtoms = tokens[0];
            numberOfBonds = tokens[1];
            numberOfMolecules = tokens[2];

            for (int i = 0; i < numberOfMolecules; i++) {
                Graph *graph = new Graph(numberOfAtoms);
                molecules.push_back(graph);
            }
        } else {
            if (bondNumber == numberOfBonds) {
                bondNumber = 0;
                moleculeNumber++;
            }

            int firstNode = tokens[0] - 1;
            int secondNode = tokens[1] - 1;
            molecules[moleculeNumber]->addEdge(firstNode, secondNode);
            
            // Increase the bond number
            bondNumber++;
        }

        // Increase line number
        lineNumber++;
    }

    // Graph *graph = molecules[0];
    // graph->condensate();
    // cout << graph->identifier() << endl;

    // return 0;
    vector<string> identifiers;
    for (Graph *graph : molecules) {
        graph->condensate();
        identifiers.push_back(graph->identifier());
    }
    sort(identifiers.begin(), identifiers.end(), compareIdentifiers);

    // for (string identifier : identifiers) {
    //     cout << identifier << endl;
    // }


    vector<int> counts(molecules.size(), 0);
    for (int i = 0; i < (int) identifiers.size(); i++) {
        string identifier = identifiers[i];

        if (i == 0) {
            counts[i] = 1;
        } else {
            int comparison = identifier.compare(identifiers[i - 1]);
            if (comparison == 0) {
                counts[i] = counts[i - 1] + 1;
            } else {
                counts[i] = 1;
            }
        }
    }

    vector<int> realCounts;
    for (int i = 0; i < numberOfMolecules; i++) {
        if (i + 1 == numberOfMolecules || counts[i] >= counts[i + 1]) {
            realCounts.push_back(counts[i]);
        }
    }

    sort(realCounts.begin(), realCounts.end());

    for (int i = 0; i < (int) realCounts.size(); i++) {
        cout << realCounts[i];
        if (i + 1 != (int) realCounts.size()) {
            cout << " ";
        }
    }
    cout << endl;

    // Clean up
    for (int i = 0; i < numberOfMolecules; i++) {
        delete molecules[i];
    }

    return 0;
}
