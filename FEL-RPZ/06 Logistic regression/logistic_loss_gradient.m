function g = logistic_loss_gradient(X, y, w)
% g = logistic_loss_gradient(X, y, w)
%
%   Calculates gradient of the logistic loss function.
%
%   Parameters:
%       X - d-dimensional observations of size [d, number_of_observations]
%       y - labels of the observations of size [1, number_of_observations]
%       w - weights of size [d, 1]
%
%   Return:
%       g - resulting gradient vector of size [d, 1]

k = X .* y;
e = k' * w;
p = @(n) (1 - (1 / (1 + exp(-n))));
result = arrayfun(p, e);
g = ((-1) * k * result) / length(X);
