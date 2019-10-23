import gurobipy as g

__author__ = 'tonda'

# SAT: (x1 v ~x2) and (~x1 v x2 v x3)

m = g.Model()
x = [m.addVar(vtype=g.GRB.BINARY) for i in range(3)]
y = [m.addVar(vtype=g.GRB.BINARY, obj=-1) for i in range(2)]

m.addConstr(x[0] + (1-x[1]) >= y[0])
m.addConstr((1-x[0]) + x[1] + x[2] >= y[1])

m.optimize()

if m.Status == g.GRB.OPTIMAL:
    print('solution:')
    print([x[i].x for i in range(3)])
    