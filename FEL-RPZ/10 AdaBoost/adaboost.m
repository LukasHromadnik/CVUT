function [strong_class, wc_error, upper_bound] = adaboost(X, y, num_steps)
% [strong_class, wc_error, upper_bound] = adaboost(X, y, num_steps)
%
% Trains an AdaBoost classifier
%
%   Parameters:
%       X [K x N] - training samples, KxN matrix, where K is the number of 
%            weak classifiers and N the number of data points
%
%       y [1 x N] - training samples labels (1xN vector, contains -1 or 1)
%
%       num_steps - number of iterations
%
%   Returns:
%       strong_class - a structure containing the found strong classifier
%           .wc [1 x num_steps] - an array containing the weak classifiers
%                  (their structure is described in findbestweak())
%           .alpha [1 x num_steps] - weak classifier coefficients
%
%       wc_error [1 x num_steps] - error of the best weak classifier in
%             each iteration
%
%       upper_bound [ 1 x num_steps] - upper bound on the training error
%

%% initialisation
N = length(y);
wc_error = zeros(1, num_steps);
upper_bound = zeros(1, num_steps);

% prepare empty arrays for results
strong_class.wc = [];
strong_class.alpha = zeros(1, num_steps);

%% AdaBoost
D = zeros(1, size(y, 2));
for i = 1:N
    D(i) = 0.5 / size(y(y == y(i)), 2);
end

for t = 1:num_steps
    disp(['Step ' num2str(t)]);
    [wc, error] = findbestweak(X, y, D);
    wc_error(t) = error;
    
    if error >= 0.5
        return
    end
    
    alpha = 0.5 * log((1 - error) / error);
    Z(t) = sum(D .* exp(alpha * y .* sign((X(wc.idx, :) - wc.theta) * wc.parity) * -1));
    D = (D .* exp(alpha * y .* sign((X(wc.idx, :) - wc.theta) * wc.parity) * -1)) ./ Z(t);
    
    strong_class.wc = [strong_class.wc, wc];
    strong_class.alpha(t) = alpha;
    upper_bound(t) = prod(Z);
end

