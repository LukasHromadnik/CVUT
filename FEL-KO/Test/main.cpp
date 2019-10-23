#include <iostream>
#include <sstream>
#include <vector>
#include <gurobi_c++.h>

using namespace std;

int main(int argc, const char * argv[])
{
    int workerCost;
    int budget;
    int numberOfProductsPerWorker;
    vector<int> demand;

    /// LOAD INPUT
    string line, word;
    int lineNumber = 0;

    ifstream inFile(argv[1]);

    // Iterate over all input lines
    while (getline(inFile, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Iterate over all "token"
        while (ss >> word) {

            // Convert input to the integer
            int input = stoi(word);

            if (lineNumber == 0) {
                workerCost = input;
            } else if (lineNumber == 1) {
                budget = input;
            } else if (lineNumber == 2) {
                numberOfProductsPerWorker = input;
            } else {
                demand.push_back(input);
            }

        }

        // Increase line number
        lineNumber++;
    }

    int numberOfHours = (int) demand.size();

    try {
    
    /// GUROBI
    GRBEnv environment;
    environment.set(GRB_IntParam_OutputFlag, 0);
    GRBModel model(environment);
    
    vector<GRBVar> x;
    vector<GRBVar> z;
    for (int i = 0; i < numberOfHours; i++) {
        // model.addVar(double lb, double ub, double obj, char type, string name)
        x.push_back(model.addVar(0, GRB_INFINITY, 0, GRB_INTEGER));
        z.push_back(model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS));
    }

    for (int i = 0; i < numberOfHours; i++) {
        GRBLinExpr lin;
        for (int j = i - 7; j < i + 1; j++) {
            int index = j < 0 ? j + 24 : j;
            lin += x[index] * numberOfProductsPerWorker;
        }

        model.addConstr(lin - demand[i] <= z[i]);
        model.addConstr(demand[i] - lin <= z[i]);
    }

    GRBLinExpr lin;
    for (int i = 0; i < (int) x.size(); i++) {
        lin += x[i] * workerCost;
    }
    model.addConstr(lin <= budget);

    GRBVar y = model.addVar(0, GRB_INFINITY, 0, GRB_BINARY);
    int M = 100;

    model.addConstr((x[3] + x[4] + x[5] + x[6] + x[7] + x[8] + x[9] + x[10]) * numberOfProductsPerWorker >= 21 - (1 - y) * M);
    model.addConstr((x[3] + x[4] + x[5] + x[6] + x[7] + x[8] + x[9] + x[10]) * numberOfProductsPerWorker <= 19 + y * M);

    GRBLinExpr objective;
    for (int i = 0; i < numberOfHours; i++) {
        objective += z[i];
    }
    model.setObjective(objective);

    model.write("callcenter.lp");
    model.optimize();

    ofstream outFile(argv[2]);
    outFile << int(model.get(GRB_DoubleAttr_ObjVal)) << endl;
    // cout << int(model.get(GRB_DoubleAttr_ObjVal)) << endl;

    /// SOLUTION
    for (int i = 0; i < numberOfHours; i++) {
        GRBVar var = x[i];
        outFile << int(var.get(GRB_DoubleAttr_X));
        // cout << int(var.get(GRB_DoubleAttr_X));
        if (i + 1 < numberOfHours) {
            outFile << " ";
            // cout << " ";
        }
    }
    // outFile << endl;

    } catch (GRBException e) {
        cout << e.getMessage() << endl;
    }

    return 0;
}
