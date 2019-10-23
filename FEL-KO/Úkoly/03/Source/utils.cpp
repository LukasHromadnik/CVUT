#include "Frame.hpp"

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

double ** computeDistances(Frame frame1, Frame frame2) {
    double ** distances = new double*[frame1.positions.size()];
    
    for (int i = 0; i < (int) frame1.positions.size(); i++) {
        distances[i] = new double[frame2.positions.size()];

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
