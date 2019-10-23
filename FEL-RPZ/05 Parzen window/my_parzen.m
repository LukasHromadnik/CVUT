function p = my_parzen(x, x_trn, h)
% p = my_parzen(x, x_trn, h)
%
%   Parzen window density estimation with normal kernel N(0, h^2).
%
%   Parameters:
%       x - vector of data points where the probability density functions
%           should be evaluated
%       x_trn - training data
%       h - kernel bandwidth
%
%   Returns:
%       p - estimated p(x|k) evaluated at values given by x

p = arrayfun(@(input) sum(normpdf(x_trn - input, 0, h))/ length(x_trn), x);
