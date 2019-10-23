#include <gurobi_c++.h>

using namespace std;

int main(int argc, char *argv[])
{
    // Create new environment.
    GRBEnv env;

    // Create empty optimization model.
    GRBModel model(env);

    // Create variables x, y.
    // addVar(lowerBound, upperBound, objectiveCoeff, variableType, name)
    GRBVar x1 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "x1");
    GRBVar x2 = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "x2");

    // Integrate new variables into model.
    model.update();

    model.addConstr(-4 * x1 - 9 * x2 <= -18);
    model.addConstr((3.0 / 2) * x1 - x2 <= (27.0 / 4));
    model.addConstr(-(8.0 / 17) * x1 + x2 <= 2);

    model.setObjective(-x1 + 2 * x2, GRB_MINIMIZE);

    model.optimize();
    // Print the objective
    // and the values of the decision variables in the solution.
    cout << "Optimal objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
    cout << "x1: " << x1.get(GRB_DoubleAttr_X) << " ";
    cout << "x2: " << x2.get(GRB_DoubleAttr_X) << endl;

    return 0;
}
