#include <gurobi_c++.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    // Create new environment.
    GRBEnv env;

    // Create empty optimization model.
    GRBModel model(env);

    vector<GRBVar> vars;
    int numberOfNodes = 5;
    for (int i = 0; i < numberOfNodes; i++) {
        GRBVar x = model.addVar(0, 1, -1, GRB_BINARY);
        vars.push_back(x);
    }

    model.update();

    vector<pair<int, int> > edges;
    edges.push_back(make_pair(0, 1));
    edges.push_back(make_pair(2, 3));
    edges.push_back(make_pair(0, 4));
    edges.push_back(make_pair(3, 1));
    edges.push_back(make_pair(3, 4));

    for (auto edge : edges) {
        model.addConstr(vars.at(edge.first) + vars.at(edge.second) <= 1);
    }

    model.optimize();

    for (auto var : vars) {
        cout << var.get(GRB_DoubleAttr_X) << endl;
    }

    return 0;
}
