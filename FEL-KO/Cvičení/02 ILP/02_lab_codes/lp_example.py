#!/usr/bin/env python3

import gurobipy as gb

m = gb.Model()

x1 = m.addVar(lb=-gb.GRB.INFINITY, ub=gb.GRB.INFINITY, vtype=gb.GRB.CONTINUOUS)
x2 = m.addVar(lb=-gb.GRB.INFINITY, ub=gb.GRB.INFINITY, vtype=gb.GRB.CONTINUOUS)

m.update()

m.addConstr(-4*x1 - 9*x2 <= -18)
m.addConstr((3/2)*x1 - x2 <= 27/4)
m.addConstr(-(8/17)*x1 + x2 <= 2)

m.setObjective(-x1 + 2*x2, sense=gb.GRB.MINIMIZE)

m.optimize()

print('')
print('Optimal solution?: ', m.status == gb.GRB.OPTIMAL)
print('Optimal objective: ', m.objVal)
print('Solution: x1 = {0}, x2 = {1}'.format(x1.x, x2.x))
