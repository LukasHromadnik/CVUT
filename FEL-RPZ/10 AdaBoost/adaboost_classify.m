function classify = adaboost_classify(strong_class, X)
% classify = adaboost_classify(strong_class, X)
%
% Applies the strong classifier to the data X and returns the
% classification labels
%
%   Parameters:
%       strong_class - the structure returned by adaboost()
%
%       X [K x N] - training samples, K is the number of weak classifiers
%            and N the number of data points
%
%   Returns:
%       classify [1 x N] - the labels of the input data X as predicted by
%            the strong classifier strong_class
%


T = length(strong_class.wc);
result = zeros(T, length(X));

for i = 1:T
    alpha = strong_class.alpha(i);
    wc = strong_class.wc(i);
    result(i, :) = sign((X(wc.idx, :) - wc.theta) * wc.parity) * alpha;
end

classify = sign(sum(result));
