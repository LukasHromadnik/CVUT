import gurobipy as g

n = 5
x = {}
for i in range(n):
    for j in range(n):
        x[i, j] = m.addVar(vtype=g.GRB.CONTINUOUS, lb=0, obj=1)

