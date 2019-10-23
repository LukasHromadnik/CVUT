function error = classification_error_discrete(data, labels, q)
% error = classification_error_discrete(images, labels, q)
%
%   Compute classification error for a discrete strategy q.
%
%   Parameters:
%       data    <1 x n> vector
%       labels  <1 x n> vector of values 1 or 2
%       q       <1 × m> vector of 1 or 2
%
%   Returns:
%       error - classification error as a fraction of false samples
%               scalar in range <0, 1>
classification = classify_discrete(data, q);
result = classification - labels;
error = sum(abs(result)) / length(result);
