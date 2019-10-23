function y = classify_images(X, w)
% y = classify_images(X, w)
%
%   Classification by logistic regression.
%
%   Parameters:
%       X - d-dimensional observations of size [d, number_of_observations]
%       w - weights of size [d, 1]
%
%   Return:
%       y - estimated labels of the observations of size [1, number_of_observations]

values = sum(w .* X);
posterior = @(e) 1 / (1 + exp(-e));
p1 = arrayfun(posterior, values);
p2 = arrayfun(posterior, -values);
result = log(p1 ./ p2);
y = double(result > 0);
y(y == 0) = -1;