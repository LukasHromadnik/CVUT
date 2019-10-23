function [mu sigma] = mle_normal(x)
% [mu sigma] = mle_normal(x)
%
%   Computes maximum likelihood estimate of mean and sigma of a normal
%   distribution.
%
%   Parameters:
%       x - input features <1 x n>
%
%   Returns:
%       mu - mean
%       sigma - standard deviation

% Mean
number_of_elements = size(x);
mu = sum(x) / number_of_elements(2);

% Standard deviation
sigma = sqrt(sum((x - mu).^2) / number_of_elements(2));

end
