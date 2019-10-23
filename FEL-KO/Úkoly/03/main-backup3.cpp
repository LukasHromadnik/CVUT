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
#include "Source/utils.cpp"
#include "Source/examples.cpp"

using namespace std;

#define ASSIGNED 'x'
#define UNASSIGNED '.'
#define NOT_ASSIGNABLE '-'
#define UNASSIGNED_ZERO '0'

#define TICKED 1
#define UNTICKED 0

template<typename Element>
void display(Element * input, int numberOfNodes) {
    for (int i = 0; i < numberOfNodes; i++) {
        cout << input[i] << " ";
    }
    cout << endl;
}

template<typename Element>
void display(vector<Element> input) {
    for (int i = 0; i < (int) input.size(); i++) {
        cout << input[i] << " ";
    }
    cout << endl;
}

struct AssignmentResult {
    char ** assigned;
    vector<int> assignedRows;
};

AssignmentResult assignment(int ** input, int numberOfNodes) {
    char ** assigned = initializeMatrix(numberOfNodes, UNASSIGNED);
    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = 0; j < numberOfNodes; j++) {
            if (input[i][j] != 0) continue;
            assigned[i][j] = UNASSIGNED_ZERO;
        }
    }

    vector<int> assignedRows(numberOfNodes, 0);

    for (int i = 0; i < numberOfNodes; i++) {
        int numberOfZeros = 0;
        int zeroIndex = -1;
        
        for (int j = 0; j < numberOfNodes; j++) {
            if (input[i][j] != 0 || assigned[i][j] == NOT_ASSIGNABLE) continue;

            numberOfZeros++;
            zeroIndex = j;
        }

        if (numberOfZeros != 1) continue;

        assigned[i][zeroIndex] = ASSIGNED;
        assignedRows[i] = 1;

        for (int j = 0; j < numberOfNodes; j++) {
            if (j == i) continue;
            assigned[j][zeroIndex] = NOT_ASSIGNABLE;
        }
    }

    for (int i = 0; i < numberOfNodes; i++) {
        int numberOfZeros = 0;
        int zeroIndex = -1;
        bool hasAssignment = false;
        
        for (int j = 0; j < numberOfNodes; j++) {
            if (input[j][i] != 0 || assigned[j][i] == NOT_ASSIGNABLE) continue;

            if (assigned[j][i] == ASSIGNED) {
                hasAssignment = true;
            }

            numberOfZeros++;
            zeroIndex = j;
        }

        if (hasAssignment) continue;

        if (numberOfZeros != 1) continue;

        assigned[zeroIndex][i] = ASSIGNED;
        assignedRows[zeroIndex] = 1;

        for (int j = 0; j < numberOfNodes; j++) {
            if (j == i) continue;
            assigned[zeroIndex][j] = NOT_ASSIGNABLE;
        }
    }

    AssignmentResult result = AssignmentResult();
    result.assigned = assigned;
    result.assignedRows = assignedRows;

    return result;
}

/**
 * Step 2: If a row is ticked and has a zero, then tick the corresponding column (if the column is not ticked yet)
 */
void step3(int ** input, char ** assigned, int numberOfNodes, vector<int> & tickedRows, vector<int> & tickedCols);

void step2(int ** input, char ** assigned, int numberOfNodes, vector<int> & tickedRows, vector<int> & tickedCols) {
    bool colsHaveChanged = false;

    for (int i = 0; i < numberOfNodes; i++) {
        // If a row is ticked
        if (tickedRows[i] == UNTICKED) continue;
        
        for (int j = 0; j < numberOfNodes; j++) {
            // And has a zero
            if (input[i][j] != 0) continue;

            // Tick the corresponding column (if the column is not ticked yet)
            if (tickedCols[j] != TICKED) {
                colsHaveChanged = true;
                tickedCols[j] = TICKED;
            }
        }
    }

    display(tickedRows);
    display(tickedCols);

    if (colsHaveChanged) {
        cout << "[COLS HAVE CHANGED]" << endl;
        step3(input, assigned, numberOfNodes, tickedRows, tickedCols);
    }
}

/**
 * Step 3: If a column is ticked and has an assignment, then tick the corresponding row (if the row is not ticked yet)
 */
void step3(int ** input, char ** assigned, int numberOfNodes, vector<int> & tickedRows, vector<int> & tickedCols) {
    bool rowsHaveChanged = false;

    for (int i = 0; i < numberOfNodes; i++) {
        // If a column is ticked
        if (tickedCols[i] != TICKED) continue;

        for (int j = 0; j < numberOfNodes; j++) {
            // And has an assignment
            if (assigned[j][i] != ASSIGNED) continue;

            // Tick the corresponding row (if the row is not ticked yet)
            if (tickedRows[j] != TICKED) {
                rowsHaveChanged = true;
                tickedRows[j] = TICKED;
            }
        }
    }

    display(tickedRows);
    display(tickedCols);

    if (rowsHaveChanged) {
        cout << "[ROWS HAVE CHANGED]" << endl;
        step2(input, assigned, numberOfNodes, tickedRows, tickedCols);
    }
}

void hungarian(int ** input, int numberOfNodes) {
    // Substract row min from all row elements
    for (int i = 0; i < numberOfNodes; i++) {
        int rowMin = INT_MAX;
        for (int j = 0; j < numberOfNodes; j++) {
            rowMin = min(rowMin, input[i][j]);
        }
        for (int j = 0; j < numberOfNodes; j++) {
            input[i][j] -= rowMin;
        }
    }

    // Substract col min from all col elements
    for (int i = 0; i < numberOfNodes; i++) {
        int colMin = INT_MAX;
        for (int j = 0; j < numberOfNodes; j++) {
            colMin = min(colMin, input[j][i]);
        }
        for (int j = 0; j < numberOfNodes; j++) {
            input[j][i] -= colMin;
        }
    }

    while (true) {

        AssignmentResult assignmentResult = assignment(input, numberOfNodes);
        char ** assigned = assignmentResult.assigned;
        vector<int> assignedRows = assignmentResult.assignedRows;

        printMatrix(assigned, numberOfNodes);
        // return;

        int numberOfAssignmets = 0;
        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (assigned[i][j] != ASSIGNED) continue;
                numberOfAssignmets++;
            }
        }

        // We found the optimal solution
        if (numberOfAssignmets == numberOfNodes) break;

        cout << "[NUMBER OF ASSIGNMENTS] " << numberOfAssignmets << endl;

        // Step 1: Tick all unassigned rows.
        vector<int> tickedRows(numberOfNodes, UNTICKED);
        for (int i = 0; i < numberOfNodes; i++) {
            tickedRows[i] = 1 - assignedRows[i];
        }

        vector<int> tickedCols(numberOfNodes, UNTICKED);

        step2(input, assigned, numberOfNodes, tickedRows, tickedCols);

        // Step 5: Draw lines through unticked rows and ticked columns. The number of lines represent the maximum number of assignments possible.
        int ** lines = initializeMatrix(numberOfNodes, 0);
        for (int i = 0; i < numberOfNodes; i++) {
            // Through unticked rows
            if (tickedRows[i] != UNTICKED) continue;
            
            for (int j = 0; j < numberOfNodes; j++) {
                lines[i][j] += 1;
            }
        }

        for (int i = 0; i < numberOfNodes; i++) {
            // Through ticked columns
            if (tickedCols[i] != TICKED) continue;
            
            for (int j = 0; j < numberOfNodes; j++) {
                lines[j][i] += 1;
            }
        }

        printMatrix(lines, numberOfNodes);

        int residualMinimum = INT_MAX;
        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (lines[i][j] != 0) continue;

                residualMinimum = min(residualMinimum, input[i][j]);
            }
        }

        cout << "[RESIDUAL MINIMUM] " << residualMinimum << endl;

        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (lines[i][j] == 0) {
                    input[i][j] -= residualMinimum;
                } else if (lines[i][j] == 2) {
                    input[i][j] += residualMinimum;
                }
            }
        }

        printMatrix(input, numberOfNodes, 5);
        cout << string(50, '-') << endl;
    }
}

int main1(int argc, const char * argv[]) {
    Example example = initializeAndSetupFirstExample();
    int numberOfNodes = example.numberOfNodes;
    int ** input = example.input;

    printMatrix(input, numberOfNodes);
    cout << "---" << endl;

    hungarian(input, numberOfNodes);

    cout << "---" << endl;

    AssignmentResult result = assignment(input, numberOfNodes);
    printMatrix(result.assigned, numberOfNodes);

    return 0;
}

int main(int argc, const char * argv[]) {

    vector<Frame> frames = loadInput(argv);

    int numberOfNodes = frames[0].positions.size();

    ofstream outFile(argv[2]);

    // for (int frameIndex = 0; frameIndex < (int) frames.size() - 1; frameIndex++) {
    for (int frameIndex = 3; frameIndex < 4; frameIndex++) {

        int ** input = computeDistances(frames[frameIndex], frames[frameIndex + 1]);
        printMatrix(input, numberOfNodes, 5);
        cout << "=====" << endl;

        hungarian(input, numberOfNodes);

        AssignmentResult result = assignment(input, numberOfNodes);
        cout << endl;
        bool firstValue = true;
        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (result.assigned[i][j] != ASSIGNED) continue;
                if (firstValue) {
                    outFile << j + 1;
                    firstValue = false;
                } else {
                    outFile << " " << j + 1;
                }
                cout << j + 1 << " ";
            }
        }
        cout << endl;
        if (frameIndex + 2 < (int) frames.size()) {
            outFile << endl;
        }
    }

    return 0;
}
