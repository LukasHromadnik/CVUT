#include <iostream>
#include <stack>
#include <vector>
#include <climits>
#include <queue>
#include <gurobi_c++.h>

#include "Source/input.cpp"
#include "Source/utils.cpp"

using namespace std;

int distanceBetween(Stripe *left, Stripe *right) {
    int distance = 0;
    for (int i = 0; i < left->height; i++) {
        for (int j = 0; j < 3; j++) {
            distance += abs(left->value(i, left->width - 1, j) - right->value(i, 0, j));
        }
    }

    return distance;
}

class Callback: public GRBCallback {
public:
    GRBVar ** vars;
    int numberOfStripes;
    int numberOfNodes;
    int ** edges;

protected:
    void callback() {
        if (where == GRB_CB_MIPSOL) {
            int * successors = initializeArray(numberOfNodes, -1);
            for (int i = 0; i < numberOfNodes; i++) {
                for (int j = 0; j < numberOfNodes; j++) {
                    if (i == j) continue;
                    int value = int(getSolution(vars[i][j]));
                    if (value == 0) continue;
                    successors[i] = j;
                }
            }

            bool * seen = initializeArray(numberOfNodes, false);

            vector<vector<int> > cycles;
            
            while (true) {
                int unseen = -1;
                for (int i = 0; i < numberOfNodes; i++) {
                    if (seen[i]) continue;
                    unseen = i;
                    break;
                }
                if (unseen == -1) break;
                cout << "[UNSEEN] " << unseen << endl;
                int succ = successors[unseen];

                vector<int> cycle;
                cycle.push_back(unseen);

                seen[unseen] = true;

                while (true) {
                    if (seen[succ]) break;
                    seen[succ] = true;
                    cycle.push_back(succ);
                    succ = successors[succ];
                }

                cycles.push_back(cycle);
                cout << "tu" << endl;
            }
            
            int minSize = INT_MAX;
            int minIndex = -1;
            for (int i = 0; i < (int) cycles.size(); i++) {
                if ((int) cycles[i].size() < minSize) {
                    minSize = cycles[i].size();
                    minIndex = i;
                }
            }

            if ((int) cycles[minIndex].size() != numberOfNodes) {
                GRBLinExpr lin;
                for (int i : cycles[minIndex]) {
                    lin += vars[i][successors[i]];
                }
                addLazy(lin, GRB_LESS_EQUAL, cycles[minIndex].size() - 1);
            }
        }
    }
};

int main(int argc, const char * argv[]) {
    Input *input = loadInput(argv);

    int ** edges = initializeMatrix(input->numberOfStripes, 0);

    for (int i = 0; i < input->numberOfStripes; i++) {
        Stripe *stripe1 = input->stripes[i];
        for (int j = 0; j < input->numberOfStripes; j++) {
            if (i == j) continue;
            Stripe *stripe2 = input->stripes[j];
            edges[i][j] = distanceBetween(stripe1, stripe2);
        }
    }
    
    printMatrix(edges, input->numberOfStripes);

    /// GUROBI
    try {
        GRBEnv environment;
        environment.set(GRB_IntParam_OutputFlag, 0);
        GRBModel model(environment);
        model.set(GRB_IntParam_LazyConstraints, 1);
        
        int numberOfNodes = input->numberOfStripes + 1;
        GRBVar ** vars = initializeMatrix(numberOfNodes, GRBVar());
        
        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (i == j) continue;
                int multiplier = (i >= input->numberOfStripes || j >= input->numberOfStripes) ? 0 : edges[i][j];
                vars[i][j] = model.addVar(0, GRB_INFINITY, multiplier, GRB_BINARY, to_string(i) + " > " + to_string(j));
            }
        }

        // for (int i = 0; i < numberOfNodes; i++) {
        //     vars[i][numberOfNodes - 1] = model.addVar(0, GRB_INFINITY, 0, GRB_BINARY, to_string(i) + " > " + to_string(numberOfNodes - 1));
        //     vars[numberOfNodes - 1][i] = model.addVar(0, GRB_INFINITY, 0, GRB_BINARY, to_string(numberOfNodes - 1) + " > " + to_string(i));
        // }

        for (int i = 0; i < numberOfNodes; i++) {
            GRBLinExpr colSum;
            GRBLinExpr rowSum;

            for (int j = 0; j < numberOfNodes; j++) {
                if (i == j) continue;
                rowSum += vars[i][j];
                colSum += vars[j][i];
            }

            model.addConstr(colSum <= 1);
            model.addConstr(colSum >= 1);

            model.addConstr(rowSum <= 1);
            model.addConstr(rowSum >= 1);
        }

        model.write("model.lp");
        Callback cb;
        cb.vars = vars;
        cb.numberOfNodes = numberOfNodes;

        model.setCallback(&cb);
        model.optimize();

        /// SOLUTION
        cout << model.get(GRB_DoubleAttr_ObjVal) << endl;

        int * successors = initializeArray(numberOfNodes, -1);
        for (int i = 0; i < numberOfNodes; i++) {
            for (int j = 0; j < numberOfNodes; j++) {
                if (i == j) continue;
                int value = int(vars[i][j].get(GRB_DoubleAttr_X));
                if (value == 0) continue;
                successors[i] = j;
            }
        }

        bool * seen = initializeArray(numberOfNodes, false);
        int startNode = numberOfNodes - 1;
        seen[startNode] = true;

        vector<int> cycle;
        cycle.push_back(startNode);
        int succ = successors[startNode];
        
        while (true) {
            if (seen[succ]) break;
            seen[succ] = true;
            cycle.push_back(succ);
            succ = successors[succ];
        }

        for (int i : cycle) {
            int j = successors[i];
            int value = (i >= input->numberOfStripes || j >= input->numberOfStripes) ? 0 : edges[i][j];
            cout << "[" << i << " -> " << j << "] " << value << endl;
        }
        cout << endl;

        cycle.erase(cycle.begin());
        ofstream outFile(argv[2]);
        bool firstValue = true;
        for (int i : cycle) {
            cout << i << " ";
            if (firstValue) {
                outFile << i + 1;
                firstValue = false;
            } else {
                outFile << " " << i + 1;
            }
        }
        cout << endl;
        outFile.close();
    } catch (GRBException e) {
        cout << e.getMessage() << endl;
    }

    return 0;
}
