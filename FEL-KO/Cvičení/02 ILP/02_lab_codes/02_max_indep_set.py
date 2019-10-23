import gurobipy as g

__author__ = 'tonda'


n = 5
E = [(0, 1), (2, 3), (0, 4), (3, 1), (3, 4)]

m = g.Model()
x = [m.addVar(vtype=g.GRB.BINARY, obj=-1) for i in range(n)]

for (i, j) in E:
    m.addConstr(x[i] + x[j] <= 1)


m.optimize()

for i in range(n):
    print(x[i].x)
