#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

enum class Operation { add, substract, multiply };

struct matrix_bad_sizes: public exception { };
struct undefined_operation: public exception { };

struct Matrix {
    using value = int;

    int numberOfRows;
    int numberOfColumns;
    value *data;

    value operator () (int row, int col) const {
        checkIndexes(row, col);

        return data[row * numberOfColumns + col];
    }

    value & operator () (int row, int col) {
        checkIndexes(row, col);

        return data[row * numberOfColumns + col];
    }

    bool operator == (const Matrix & m) {
        if (numberOfRows != m.numberOfRows) return false;
        if (numberOfColumns != m.numberOfColumns) return false;
        for (int i = 0; i < numberOfColumns * numberOfRows; i++)
            if (data[i] != m.data[i])
                return false;

        return true;
    }
    
    Matrix & operator = (const Matrix & m) {
        if (*this == m) return *this;

        numberOfRows = m.numberOfRows;
        numberOfColumns = m.numberOfColumns;
        int numberOfValues = numberOfRows * numberOfColumns;
        data = new value[numberOfValues];
        for (int i = 0; i < numberOfValues; i++)
            data[i] = m.data[i];

        return *this;
    }

    Matrix() : Matrix(0, 0) { }
    Matrix(int numberOfRows, int numberOfColumns) : numberOfRows(numberOfRows), numberOfColumns(numberOfColumns) {
        data = new value[numberOfRows * numberOfColumns];
        for (int i = 0; i < numberOfRows * numberOfColumns; i++)
            data[i] = 0;
    }
    ~Matrix() {
        delete[] data;
    }

private:
    void checkIndexes(int row, int col) const {
        if (row >= numberOfRows)
            throw out_of_range("Row index is out of range");
        if (col >= numberOfColumns)
            throw out_of_range("Column index is out of range");
    }
};

Matrix operator + (const Matrix & lhs, const Matrix & rhs) {
    Matrix result = Matrix(lhs.numberOfRows, lhs.numberOfColumns);
    for (int i = 0; i < lhs.numberOfRows; i++)
        for (int j = 0; j < lhs.numberOfColumns; j++)
            result(i, j) = lhs(i, j) + rhs(i, j);

    return result;
}

Matrix operator - (const Matrix & lhs, const Matrix & rhs) {
    Matrix result = Matrix(lhs.numberOfRows, lhs.numberOfColumns);
    for (int i = 0; i < lhs.numberOfRows; i++)
        for (int j = 0; j < lhs.numberOfColumns; j++)
            result(i, j) = lhs(i, j) - rhs(i, j);

    return result;
}

Matrix operator * (const Matrix & lhs, const Matrix & rhs) {
    Matrix result = Matrix(lhs.numberOfRows, rhs.numberOfColumns);
    for (int i = 0; i < lhs.numberOfRows; i++)
        for (int j = 0; j < rhs.numberOfColumns; j++)
            for (int k = 0; k < lhs.numberOfColumns; k++)
                result(i, j) = result(i, j) + lhs(i, k) * rhs(k, j);

    return result;
}

ostream & operator << (ostream & os, const Matrix & m) {
    for (int i = 0; i < m.numberOfRows; i++) {
        for (int j = 0; j < m.numberOfColumns; j++)
            os << m(i, j) << " ";
        
        os << endl;
    }

    return os;
}

bool checkMatrixSizesForOperation(const Matrix & m1, const Matrix & m2, Operation op) {
    switch (op) {
        case Operation::add:
        case Operation::substract:
            return m1.numberOfRows == m2.numberOfRows && m1.numberOfColumns == m2.numberOfColumns;
        case Operation::multiply:
            return m1.numberOfColumns == m2.numberOfRows;
    }
}

stringstream loadLine() {
    string line;
    cout << "> ";
    getline(cin, line);
    stringstream ss(line);

    return ss;
}

vector<int> loadMatrixSize() {
    stringstream ss = loadLine();
    string word;
    vector<int> tokens;
    while (ss >> word) {
        tokens.push_back(stoi(word));
    }

    return tokens;
}

void loadMatrixData(Matrix & m) {
    string word;
    for (int i = 0; i < m.numberOfRows; i++) {
        stringstream ss = loadLine();
        int counter = 0;
        while (ss >> word && counter < m.numberOfColumns) {
            m(i, counter) = stoi(word);
            counter++;
        }
    }
}

Operation loadOperation() {
    string word;
    stringstream ss = loadLine();
    ss >> word;
    
    if (word == "+")
        return Operation::add;
    else if (word == "-")
        return Operation::substract;
    else if (word == "*")
        return Operation::multiply;
    else
        throw undefined_operation();
}

int main() {
    cout << "=== POČÍTÁNÍ S MATICEMI ===" << endl;
    cout << "Zadejte rozměry první matice ve formátu: počet_řádků počet_sloupců" << endl;

    vector<int> size = loadMatrixSize();
    Matrix m1 = Matrix(size[0], size[1]);

    cout << endl << "Zadejte řádky matice, čísla oddělená mezerou a řádky enterem." << endl;

    loadMatrixData(m1);

    cout << endl << "Zadejte operaci, kterou chcete s maticemi provést:" << endl;
    cout << " + součet" << endl;
    cout << " - rozdíl" << endl;
    cout << " * součin" << endl;

    Operation op = loadOperation();

    cout << endl << "Zadejte rozměry druhé matice ve formátu: počet_řádků počet_sloupců" << endl;

    size = loadMatrixSize();
    Matrix m2 = Matrix(size[0], size[1]);

    cout << endl << "Zadejte řádky matice, čísla oddělená mezerou a řádky enterem." << endl;

    loadMatrixData(m2);

    if (checkMatrixSizesForOperation(m1, m2, op) == false)
        throw matrix_bad_sizes();

    Matrix result;
    switch (op) {
        case Operation::add:
            result = m1 + m2;
            break;
        case Operation::substract:
            result = m1 - m2;
            break;
        case Operation::multiply:
            result = m1 * m2;
            break;
    }

    cout << endl << "Výsledek:" << endl << result << endl;

    return 0;
}
