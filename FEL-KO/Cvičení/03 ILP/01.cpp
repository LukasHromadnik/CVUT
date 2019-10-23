#include <gurobi_c++.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    vector<pair<string, int> > input;
    input.push_back(make_pair("A", 0));
    input.push_back(make_pair("B", 590));
    input.push_back(make_pair("N", 110));
    input.push_back(make_pair("J", 300));

    int sum = 0;
    for (auto person : input) {
        sum += person.second;
    }
    int mean = sum / input.size();

    // Create new environment.
    GRBEnv env;

    // Create empty optimization model.
    GRBModel model(env);

    GRBVar ** vars = new GRBVar * [(int) input.size()];
    for (int i = 0; i < input.size(); i++) {
        vars[i] = new GRBVar[(int) input.size()];
        for (int j = 0; j < input.size(); j++) {
            GRBVar x = model.addVar(0, 1, 0, GRB_INTEGER);
            vars[i][j] = x;
        }
    }

    model.update();

    model.setObjective(GRBLinExpr(0), GRB_MINIMIZE);

    for (int i = 0; i < input.size(); i++) {
        GRBLinExpr lin = input[i].second;
        for (int j = 0; j < input.size(); j++) {
            if (i == j) continue;
            GRBLinExpr var = GRBLinExpr(vars[i][j], 1);
            lin += var;
        }
        // lin += GRBLinExpr(input[i].second);
        for (int j = 0; j < input.size(); j++) {
            if (i == j) continue;
            GRBLinExpr var = GRBLinExpr(vars[j][i], 1);
            lin += var;
        }
        model.addConstr(lin == mean);
    }

    model.optimize();

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input.size(); j++) {
            cout << vars[i][j].get(GRB_DoubleAttr_X) << endl;
        }
    }

    return 0;
}
