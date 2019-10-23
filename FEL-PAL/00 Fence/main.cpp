#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

int main(int argc, const char * argv[]) {

    string line, text, word;
    int lineNumber = 0;
    int numberOfCoords = 0;
    int ratio = 5000;
    int millimetersInMeter = 1000;

    int **coords;

    // Iterate over all input lines
    while (getline(cin, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Declare coords number
        int coordsNumber = 0;

        // Iterate over all "token"
        while (ss >> word) {

            // Convert input to the integer
            int input = stoi(word);

            // Set number of the coords
            if (lineNumber == 0) {
                numberOfCoords = input + 1;

                // Initialize coords array
                coords = new int*[numberOfCoords];
                for (int i = 0; i < numberOfCoords; i++) {
                    coords[i] = new int[2];
                }
            }
            
            // Load coordinates
            else {
                coords[lineNumber - 1][coordsNumber++] = input;
            }
        }

        // Increase line number
        lineNumber++;
    }

    // Copy the first coordinates to the last position
    coords[numberOfCoords - 1][0] = coords[0][0];
    coords[numberOfCoords - 1][1] = coords[0][1];

    // Initialize accumulator for the result
    double sum = 0;
 
    // Compute size between given coordinates
    for (int i = 1; i < numberOfCoords; i++) {
        double firstCoord = pow(coords[i][0] - coords[i - 1][0], 2);
        double secondCoord = pow(coords[i][1] - coords[i - 1][1], 2);

        sum += sqrt(firstCoord + secondCoord);
    }

    // Convert computation result to the real life units
    double length = sum * ratio;

    // Convert milimeters to the meters
    double realLength = length / millimetersInMeter;
    
    // Return result rounded up
    cout << ceil(realLength) << endl;

    // Clean up memory
    for (int i = 0; i < numberOfCoords; i++) {
        delete[] coords[i];
    }
    delete[] coords;

    return 0;
}
