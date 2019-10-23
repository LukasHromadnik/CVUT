#include <iostream>
#include <iomanip>

using namespace std;

template<typename Matrix>
void printMatrix(Matrix ** graph, int numberOfNodes, int colWidth = 4, int firstColWidth = 2) {
    cout << string(firstColWidth, ' ') << " |";
    // Header
    for (int i = 0; i < numberOfNodes; i++) {
        cout << setw(colWidth) << i << " ";
    }
    cout << endl;
    
    // Separator
    cout << string(firstColWidth + 1, '-') << "+" << string(colWidth * numberOfNodes + numberOfNodes - 1, '-') << endl;
    
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

template<typename Element>
void printArray(Element * data, int numberOfNodes) {
    for (int i = 0; i < numberOfNodes; i++) {
        cout << data[i] << " ";
    }
    cout << endl;
}

template<typename Element>
void printVector(vector<Element> data) {
    for (int i = 0; i < (int) data.size(); i++) {
        cout << data[i] << " ";
    }
    cout << endl;
}

template<typename Element>
Element * initializeArray(int numberOfNodes, Element initialValue) {
    Element * array = new Element [numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++) {
        array[i] = initialValue;
    }

    return array;
}

template<typename Element>
Element ** initializeMatrix(int numberOfNodes, Element initialValue) {
    Element ** matrix = new Element * [numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++) {
        matrix[i] = initializeArray(numberOfNodes, initialValue);
    }

    return matrix;
}
