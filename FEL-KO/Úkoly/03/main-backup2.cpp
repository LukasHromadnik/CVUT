#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>

#include "Source/Position.cpp"
#include "Source/Frame.cpp"
// #include "Source/Edge.cpp"
// #include "Source/Graph.cpp"

// #include "Source/BellmanFord.cpp"
// #include "Source/FordFulkerson.cpp"

using namespace std;

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

template<typename Matrix>
void printMatrix(Matrix ** graph, int numberOfNodes, int colWidth = 3, int firstColWidth = 2) {
    cout << string(firstColWidth, ' ') << " |";
    // Header
    for (int i = 0; i < numberOfNodes; i++) {
        cout << setw(colWidth) << i << " ";
    }
    cout << endl;
    
    // Separator
    cout << string(colWidth * numberOfNodes + numberOfNodes + firstColWidth + 1, '-') << endl;
    
    // Data
    for (int i = 0; i < numberOfNodes; i++) {
        cout << setw(firstColWidth) << i << " |";
        for (int j = 0; j < numberOfNodes; j++) {
            cout << setw(colWidth) << graph[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << endl;
}

template<typename Output>
Output ** initializeMatrix(int numberOfNodes, Output initialValue) {
    Output ** array = new Output * [numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++) {
        array[i] = new Output [numberOfNodes];
        for (int j = 0; j < numberOfNodes; j++) {
            array[i][j] = initialValue;
        }
    }

    return array;
}

int main(int argc, const char * argv[]) {
    // Load input
    vector<Frame> frames = loadInput(argv);

    ofstream outFile(argv[2]);

    for (int frameIndex = 0; frameIndex < (int) frames.size() - 1; frameIndex++) {
        // int frameIndex = 1;
        Frame frame1 = frames[frameIndex];
        Frame frame2 = frames[frameIndex + 1];

        int numberOfFirstPositions = frame1.positions.size();
        int numberOfSecondPositions = frame2.positions.size();

        // Save number of points in the frame
        int numberOfNodes = numberOfFirstPositions + numberOfSecondPositions + 2;

        // Create lower bounds
        int ** lowerBounds = initializeMatrix(numberOfNodes, 0);
        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (i == 0 || j == numberOfNodes - 1) {
                    lowerBounds[i][j] = 1;
                }
            }
        }

        // Create upper bounds
        int ** upperBounds = initializeMatrix(numberOfNodes, 1);

        // Compute distances
        int ** positionDistances = computeDistances(frame1, frame2);

        // Create costs
        int ** costs = initializeMatrix(numberOfNodes, 0);
        for (int i = 0; i < numberOfFirstPositions; i++) {
            for (int j = 0; j < numberOfSecondPositions; j++) {
                int startNode = i + 1;
                int endNode = j + numberOfFirstPositions + 1;
                costs[startNode][endNode] = positionDistances[i][j];
            }
        }
        
        // Create graph
        int ** graph = initializeMatrix(numberOfNodes, 0);

        for (int i = 0; i < numberOfFirstPositions; i++) {
            int index = i + 1;
            graph[0][index] = -1;
            graph[index][0] = 1;
        }

        for (int i = 0; i < numberOfSecondPositions; i++) {
            int startIndex = i + numberOfFirstPositions + 1;
            int endIndex = numberOfNodes - 1;
            graph[startIndex][endIndex] = -1;
            graph[endIndex][startIndex] = 1;
        }

        for (int i = 0; i < numberOfFirstPositions; i++) {
            for (int j = 0; j < numberOfSecondPositions; j++) {
                int startIndex = i + 1;
                int endIndex = j + numberOfFirstPositions + 1;
                graph[startIndex][endIndex] = -1;
                graph[endIndex][startIndex] = 1;
            }
        }

        // Create flow
        int ** flows = initializeMatrix(numberOfNodes, 0);

        for (int i = 0; i < numberOfFirstPositions; i++) {
            int index = i + 1;
            flows[0][index] = 1;
        }

        for (int i = 0; i < numberOfSecondPositions; i++) {
            int startIndex = i + numberOfFirstPositions + 1;
            int endIndex = numberOfNodes - 1;
            flows[startIndex][endIndex] = 1;
        }

        for (int i = 0; i < numberOfFirstPositions; i++) {
            int startIndex = i + 1;
            int endIndex = i + numberOfFirstPositions + 1;
            flows[startIndex][endIndex] = 1;
        }

        // Create residuals
        int ** residuals = initializeMatrix(numberOfNodes, 0);

        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (graph[i][j] != -1) continue;
                residuals[i][j] = upperBounds[i][j] - flows[i][j];
                residuals[j][i] = flows[i][j] - lowerBounds[i][j];
            }
        }

        // Create residual graph
        int residualNumberOfNodes = numberOfNodes + 1;
        int ** residualGraph = initializeMatrix(residualNumberOfNodes, 0);
        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (residuals[i][j] == 0) continue;
                residualGraph[i][j] = -1;
                residualGraph[j][i] = 1;
            }
            residualGraph[numberOfNodes][i] = -1;
            residualGraph[i][numberOfNodes] = 1;
        }

        // Create residual costs
        int ** residualCosts = initializeMatrix(residualNumberOfNodes, 0);
        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = i + 1; j < numberOfNodes; j++) {
                residualCosts[i][j] = costs[i][j];
                residualCosts[j][i] = -costs[i][j];
            }
        }
        
        // printMatrix(residuals, numberOfNodes, 5);

        while (true) {
            
            // BELLMAN-FORD
            int * distances = new int [residualNumberOfNodes];
            for (int i = 0; i < residualNumberOfNodes; i++) {
                distances[i] = INT_MAX;
            }

            int * predecessors = new int [residualNumberOfNodes];
            for (int i = 0; i < residualNumberOfNodes; i++) {
                predecessors[i] = -1;
            }

            int initialNode = 8;
            distances[initialNode] = 0;

            // Costs relaxation
            for (int a = 0; a < residualNumberOfNodes; a++) {
                for (int i = 0; i < residualNumberOfNodes; i++) {
                    for (int j = 0; j < residualNumberOfNodes; j++) {
                        if (residualGraph[i][j] != -1) continue;

                        if (distances[i] != INT_MAX && distances[i] + residualCosts[i][j] < distances[j]) {
                            distances[j] = distances[i] + residualCosts[i][j];
                            predecessors[j] = i;
                        }
                    }
                }
            }

            bool foundNegativeCycle = false;
            
            for (int i = 0; i < residualNumberOfNodes; i++) {
                for (int j = 0; j < residualNumberOfNodes; j++) {
                    if (residualGraph[i][j] != -1) continue;

                    if (distances[i] + residualCosts[i][j] < distances[j]) {
                        // cout << "[" << i << " " << j << "] " << distances[i] << " + " << residualCosts[i][j] << " < " << distances[j] << endl;
                        // cout << "Graph contains a negative-weight cycle" << endl;
                        foundNegativeCycle = true;
                        break;
                    }
                }
                if (foundNegativeCycle) break;
            }

            if (foundNegativeCycle) {
                // for (int i = 0; i < residualNumberOfNodes; i++) {
                //     cout << predecessors[i] << " ";
                // }
                // cout << endl;

                queue<int> preds;
                preds.push(1);

                vector<int> seen(residualNumberOfNodes, 0);

                foundNegativeCycle = false;

                int pred = -1;
                // cout << "tu1" << endl;
                while ( ! foundNegativeCycle) {
                    pred = preds.front();
                    preds.pop();
                    // cout << "[POP] " << pred << endl;

                    seen[pred] = 1;

                    for (int i = 0; i < residualNumberOfNodes; i++) {
                        if (i != predecessors[pred]) continue;

                        preds.push(i);

                        if (seen[i] == 1) {
                            foundNegativeCycle = true;
                            pred = i;
                        }

                        break;
                    }
                }
                // cout << "tu2" << endl;
                stack<int> cycleStack;
                int lastPred = pred;
                cycleStack.push(pred);
                
                do {
                    pred = predecessors[pred];
                    cycleStack.push(pred);
                } while (lastPred != pred);

                vector<int> cycle;
                while ( ! cycleStack.empty()) {
                    cycle.push_back(cycleStack.top());
                    cycleStack.pop();
                }
                
                int residuum = INT_MAX;
                for (int i = 0; i < cycle.size() - 1; i++) {
                    int startNode = cycle[i];
                    int endNode = cycle[i + 1];
                    residuum = min(residuum, residuals[startNode][endNode]);
                }

                if (residuum == 0) break;

                // printMatrix(flows, numberOfNodes);
                // printMatrix(residuals, numberOfNodes);

                for (int i = 0; i < cycle.size() - 1; i++) {
                    int startNode = cycle[i];
                    int endNode = cycle[i + 1];

                    // cout << startNode << " " << endNode << " " << residuum << endl;

                    if (graph[startNode][endNode] == -1) {
                        flows[startNode][endNode] += residuum;
                    } else {
                        flows[endNode][startNode] -= residuum;
                    }

                    residuals[startNode][endNode] -= residuum;
                    residuals[endNode][startNode] += residuum;
                }
            } else {
                break;
            }

            // printMatrix(flows, numberOfNodes);
            // cout << "---" << endl;

            // cout << "end" << endl;
        }

        bool firstValue = true;
        for (int i = 0; i < numberOfFirstPositions; i++) {
            for (int j = 0; j < numberOfSecondPositions; j++) {
                int startIndex = i + 1;
                int endIndex = j + numberOfFirstPositions + 1;

                if (flows[startIndex][endIndex] != 1) continue;
                if (firstValue) {
                    outFile << j + 1;
                    firstValue = false;
                } else {
                    outFile << " " << j + 1;
                }
                cout << j + 1 << " ";
            }
        }
        if (frameIndex + 2 != (int) frames.size()) {
            outFile << endl;
        }
        cout << endl;
    }

    outFile.close();

    return 0;
}
