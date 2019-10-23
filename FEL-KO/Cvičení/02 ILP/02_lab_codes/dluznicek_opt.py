#!/usr/bin/env python3

import gurobipy as gb

m = gb.Model()

P = set(["A", "B", "N", "J"])
c = {"A": 0, "B": 590, "N": 110, "J": 300}

s = sum(c.values()) / len(c)

a = dict()
for p in P:
    for q in P:
        a[p, q] = m.addVar(lb=0, ub=gb.GRB.INFINITY, vtype=gb.GRB.CONTINUOUS)

y = dict()
for p in P:
    for q in P:
        y[p, q] = m.addVar(vtype=gb.GRB.BINARY)

m.update()

for p in P:
    m.addConstr(\
        gb.quicksum(a[p, q] for q in P)\
        + c[p]\
        - gb.quicksum(a[q, p] for q in P) == s
    )

M = sum(c.values())
for p in P:
    for q in P:
        m.addConstr(y[p, q] <= a[p, q])
        m.addConstr(a[p, q] <= y[p, q] * M)

m.setObjective(gb.quicksum(y[p, q] for p in P for q in P), sense=gb.GRB.MINIMIZE)

m.optimize()

for p in P:
    for q in P:
        print("{0} -> {1}: {2}".format(p, q, a[p, q].X))
