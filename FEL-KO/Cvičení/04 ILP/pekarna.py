import gurobipy as g

n = 3
orders = {
    0: {'r': 20, 'd': 45, 'p': 15},
    1: {'r': 4, 'd': 30, 'p': 19},
    2: {'r': 5, 'd': 80, 'p': 20},
    3: {'r': 17, 'd': 70, 'p': 8},
    4: {'r': 27, 'd': 66, 'p': 7}
}

m = g.Model()

s = [m.addVar(vtype=g.GRB.CONTINUOUS, lb=orders[i]['r'], ub=orders[i]['d'] - orders[i]['p']) for i in range(n)]
x = {}
for i in range(n):
    for j in range(n):
        x[i, j] = m.addVar(vtype=g.GRB.BINARY)

Cmax = m.addVar(vtype=g.GRB.CONTINUOUS, obj=1)

M = max([orders[i]['d'] for i in range(n)])

for i in range(n):
    m.addConstr(s[i] + orders[i]['p'] <= Cmax)

for i in range(n):
    for j in range(n):
        m.addConstr(s[i] + orders[i]['p'] <= s[j] + M * (1 - x[i, j]))
        m.addConstr(s[j] + orders[j]['p'] <= s[i] + M * x[i, j])

m.optimize()

# for i in range(n):
#     print('order {} starts baking at {}'.format(i + 1, int(round(s[i].x))))
