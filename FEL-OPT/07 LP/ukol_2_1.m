load data1;

f = [0 0 1];

A = [
         x,        ones(m, 1),   -1 * ones(m, 1);
    -1 * x,   -1 * ones(m, 1),   -1 * ones(m, 1)
    ];

b = [y' -y'];

solution = linprog(f, A, b)

hold on
scatter(x, y);

a = solution(1);
b = solution(2);
e = solution(3);

graphs = [
    b + e, a + b + e;
    b,     a + b;
    b - e, a + b - e
];

plot([0 1], graphs);

hold off
