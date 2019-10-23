f = {}
y = {}

for p in P:
    for q in P:
        f[p, q] = m.addVar(vtype=g.GRB.INTEGER, lb=0)
        y[p, q] = m.addVar(vtype=g.GRB.BINARY, obj=1)

M = sum(c.values())
M = 1000000000000

for p in P:
    m.addConstr(g.quicksum([f[p, q] for q in P]) + c[p] - g.quicksum([f[q, p] for q in P]) == s)

for p in P:
    for q in P:
        m.addConstr(f[p, q] <= y[p, q] * M)
        m.addConstr(y[p, q] <= f[p, q])

m.optimize()

print([y[p, q].x for p in P or q in P])

for p in P:
    for q in P:
        print('{} -> {}: {}'.format(p, q, f[p, q].x]))