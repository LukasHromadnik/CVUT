#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include "Source/Position.cpp"
#include "Source/Frame.cpp"
#include "Source/Edge.cpp"
#include "Source/Graph.cpp"

#include "Source/BellmanFord.cpp"
#include "Source/FordFulkerson.cpp"

using namespace std;

class MyGraph: public Graph {
public:
    int numberOfPositionsInFirstFrame;
    int numberOfPositionsInSecondFrame;
    
    MyGraph(int numberOfPositionsInFirstFrame, int numberOfPositionsInSecondFrame);

    int firstFramePositionIndexToIndex(int index);
    int secondFramePositionIndexToIndex(int index);
};

MyGraph::MyGraph(int numberOfPositionsInFirstFrame, int numberOfPositionsInSecondFrame): Graph(numberOfPositionsInFirstFrame + numberOfPositionsInSecondFrame + 2) {
    this->numberOfPositionsInFirstFrame = numberOfPositionsInFirstFrame;
    this->numberOfPositionsInSecondFrame = numberOfPositionsInSecondFrame;
}

int MyGraph::firstFramePositionIndexToIndex(int index) {
    return index + 1;
}

int MyGraph::secondFramePositionIndexToIndex(int index) {
    return this->numberOfPositionsInFirstFrame + index + 1;
}

int ** computeDistances(Frame frame1, Frame frame2) {
    int ** distances = new int*[frame1.positions.size()];
    
    for (int i = 0; i < (int) frame1.positions.size(); i++) {
        distances[i] = new int[frame2.positions.size()];

        Position pos1 = frame1.positions[i];
        
        for (int j = 0; j < (int) frame2.positions.size() ; j++) {
            Position pos2 = frame2.positions[j];
            distances[i][j] = pos1.distanceFromPosition(pos2);
        }
    }

    return distances;
}

Graph * buildResidualGraph(Graph *graph) {
    // Create new graph with one new source node
    Graph *residualGraph = new Graph(graph->numberOfNodes + 1);

    // Iterate over all edges
    for (int i = 0; i < graph->numberOfNodes; i++) {
        for (Edge *edge : graph->edges[i]) {
            // If the edge does not exist or the node is not the start node, skip it
            if (edge == NULL || edge->start != i) continue;

            int startNode = edge->start + 1;
            int endNode = edge->end + 1;

            // Create forward edge
            Edge *forwardEdge = new Edge(startNode, endNode);
            forwardEdge->cost = edge->cost;
            forwardEdge->value = edge->upperBound - edge->value;
            residualGraph->addEdge(forwardEdge);

            // Create backward edge
            Edge *backwardEdge = new Edge(endNode, startNode);
            backwardEdge->cost = -edge->cost;
            backwardEdge->value = edge->value - edge->lowerBound;
            residualGraph->addEdge(backwardEdge);
        }
    }

    for (int i = 1; i < residualGraph->numberOfNodes; i++) {
        for (int j = 1; j < residualGraph->numberOfNodes; j++) {
            Edge *edge = residualGraph->edges[i][j];
            if (edge == NULL || edge->value > 0) continue;
            residualGraph->edges[i][j] = NULL;
        }

        residualGraph->addEdge(new Edge(residualGraph->startIndex(), i));
    }

    // for (int i = 0; i < residualGraph->numberOfNodes; i++) {
    //     for (int j = 0; j < residualGraph->numberOfNodes; j++) {
    //         Edge *edge = residualGraph->edges[i][j];
    //         if (edge == NULL) {
    //             cout << ".";
    //         } else {
    //             cout << residualGraph->edges[i][j]->value;
    //         }
    //         cout << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    return residualGraph;
}

vector<Frame> loadInput(const char *argv[]) {
    // Initial values
    vector<Frame> frames;

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

        int cache = -1;

        Frame frame = Frame();

        // Iterate over all "token"
        while (ss >> word) {

            // Convert input to the integer
            int input = stoi(word);

            if (lineNumber > 0) {
                if (wordNumber % 2 == 0) {
                    cache = input;
                } else {
                    frame.addPosition(cache, input);
                }
            }

            // Increase word number
            wordNumber++;
        }

        if (lineNumber > 0) {
            frames.push_back(frame);
        }

        // Increase line number
        lineNumber++;
    }

    inFile.close();

    return frames;
}

int main(int argc, const char * argv[]) {

    // Initial values
    vector<Frame> frames = loadInput(argv);

    bool useDebug = false;
    ofstream outFile(argv[2]);
    
    if (argv[2] == NULL) {
        useDebug = true;
    }

    for (int frameIndex = 0; frameIndex < (int) frames.size() - 1; frameIndex++) {
        Frame frame1 = frames[frameIndex];
        Frame frame2 = frames[frameIndex + 1];

        // Compute distances between given frames
        int ** distances = computeDistances(frame1, frame2);

        // Create minCostFlow graph for given frames
        MyGraph *graph = new MyGraph(frame1.positions.size(), frame2.positions.size());

        // Create edges between start node and the first frame positions
        for (int i = 0; i < graph->numberOfPositionsInFirstFrame; i++) {
            Edge *edge = new Edge(graph->startIndex(), graph->firstFramePositionIndexToIndex(i));
            edge->lowerBound = 1;
            edge->upperBound = 1;
            graph->addEdge(edge, true);
        }

        // Create edges between positions from the second frame and the end node
        for (int i = 0; i < graph->numberOfPositionsInSecondFrame; i++) {
            Edge *edge = new Edge(graph->secondFramePositionIndexToIndex(i), graph->endIndex());
            edge->lowerBound = 1;
            edge->upperBound = 1;
            graph->addEdge(edge, true);
        }
        
        // Create edges between given frames positions
        for (int i = 0; i < graph->numberOfPositionsInFirstFrame; i++) {
            for (int j = 0; j < graph->numberOfPositionsInSecondFrame; j++) {
                int firstIndex = graph->firstFramePositionIndexToIndex(i);
                int secondIndex = graph->secondFramePositionIndexToIndex(j);
                Edge *edge = new Edge(firstIndex, secondIndex);
                edge->upperBound = 1;
                edge->cost = distances[i][j];
                graph->addEdge(edge, true);
            }
        }

        // Attempt to create initial feasible flow
        // Flow will be always feasible
        graph = graphWithFeasibleFlow(graph);

        while (true) {

            // Create residual graph for given flow
            Graph *residualGraph = buildResidualGraph(graph);

            // Detect negative cycle
            vector<pair<int, int> > negativeCycle = BellmanFord(residualGraph);

            // Initialize improving capacity
            int improvingCapacity = INT_MAX;
            
            // If there is negative cycle, solution was found
            if (negativeCycle.empty()) break;

            // Compute the improving capacity
            for (pair<int, int> nodes : negativeCycle) {
                improvingCapacity = min(residualGraph->edges[nodes.first][nodes.second]->value, improvingCapacity);
            }

            // Nothing to improve, solution found
            if (improvingCapacity == 0) break;
            
            // Update flow on edges from the negative cycle
            for (pair<int, int> nodes : negativeCycle) {
                int u = nodes.first - 1;
                int v = nodes.second - 1;

                if (u < 0 || v < 0) continue;

                Edge *edge = graph->edges[u][v];
                if (edge->start == u) {
                    edge->value += improvingCapacity;
                } else {
                    edge->value -= improvingCapacity;
                }
            }
        }

        for (int i = 0; i < graph->numberOfPositionsInFirstFrame; i++) {
            int index = graph->firstFramePositionIndexToIndex(i);
            for (Edge *edge : graph->edges[index]) {
                if (edge == NULL || edge->start != index || edge->value == 0) continue;
                outFile << edge->end - graph->numberOfPositionsInFirstFrame;
                if (useDebug) {
                    cout << edge->end - graph->numberOfPositionsInFirstFrame << " ";
                }
            }
            if (i + 1 != graph->numberOfPositionsInFirstFrame) {
                outFile << " ";
            } else if (frameIndex + 2 != (int) frames.size()) {
                outFile << endl;
            }
        }
        if (useDebug) {
            cout << endl;
        }
    }

    outFile.close();

    return 0;
}

int main(int argc, const char * argv[]) {
    vector<Frame> frames = loadInput(argv);

    Frame frame1 = frames[8];
    Frame frame2 = frames[9];

    int ** distances = computeDistances(frame1, frame2);

    // for (int i = 0; i < frame1.positions.size(); i++) {
    //     for (int j = 0; j < frame1.positions.size(); j++) {
    //         cout << distances[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    Graph *graph = new Graph(frame1.positions.size() + frame2.positions.size() + 2);
    for (int i = 0; i < frame1.positions.size(); i++) {
        int startNode = i + 1;
        Edge *edge = new Edge(graph->startIndex(), startNode);
        edge->lowerBound = 1;
        edge->upperBound = 1;
        graph->addEdge(edge);
    }

    for (int j = 0; j < frame2.positions.size(); j++) {
        int endNode = j + frame1.positions.size() + 1;
        Edge *edge = new Edge(endNode, graph->endIndex());
        edge->lowerBound = 1;
        edge->upperBound = 1;
        graph->addEdge(edge);
    }

    for (int i = 0; i < frame1.positions.size(); i++) {
        for (int j = 0; j < frame2.positions.size(); j++) {
            int startNode = i + 1;
            int endNode = j + frame1.positions.size() + 1;
            Edge *edge = new Edge(startNode, endNode);
            edge->upperBound = 1;
            edge->cost = distances[i][j];
            graph->addEdge(edge);
        }
    }

    graph->print();
    cout << endl;

    graph = graphWithFeasibleFlow(graph);
    
    graph->print();
    cout << endl;

    Graph *residualGraph = new Graph(graph->numberOfNodes);
    for (int i = 0; i < graph->numberOfNodes; i++) {
        for (int j = 0; j < graph->numberOfNodes; j++) {
            Edge *currentEdge = graph->edges[i][j];
            if (currentEdge == NULL) continue;

            Edge *forwardEdge = new Edge(i, j);
            forwardEdge->lowerBound = currentEdge->lowerBound;
            forwardEdge->upperBound = currentEdge->upperBound;
            forwardEdge->value = currentEdge->upperBound - currentEdge->value;
            forwardEdge->cost = currentEdge->cost;
            residualGraph->addEdge(forwardEdge);

            Edge *backwardEdge = new Edge(j, i);
            backwardEdge->lowerBound = currentEdge->lowerBound;
            backwardEdge->upperBound = currentEdge->upperBound;
            backwardEdge->value = currentEdge->value - currentEdge->lowerBound;
            backwardEdge->cost = currentEdge->cost;
            residualGraph->addEdge(backwardEdge);
        }
    }

    graph->print();
    cout << endl;





    return 0;
}
