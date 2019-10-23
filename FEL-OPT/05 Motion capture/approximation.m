function [A_approx, base] = approximation(A, rank)

m = size(A, 1); % number of rows
n = size(A, 2); % number of cols

center_of_gravity = sum(A) / m;

A0 = A - center_of_gravity; % translation to the origin

[V, ~] = eig(A0' * A0); % spectral decomposition
base = V(:,end - rank + 1:end);
projector = base * inv(base' * base) * base';
projection = projector * A0';
A_approx = projection' + center_of_gravity;

end