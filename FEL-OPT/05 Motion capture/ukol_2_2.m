A = load('walk1.txt','-ASCII');

m = size(A, 1); % number of rows
n = size(A, 2); % number of cols

center_of_gravity = sum(A) / m;

A0 = A - center_of_gravity; % translation to the origin

[~, D] = eig(A0' * A0); % spectral decomposition
eigenvalues = sum(D);

ranks = [1 2 5 10 15];
criterions = arrayfun(@(x) sum(eigenvalues(1:end - x)), ranks);
