function idx = random_sample(weights)
% RANDOM_SAMPLE picks randomly a sample based on the sample weights.
%
% Suppose weights / sum(weights) is a discrete probability distribution. 
% RANDOM_SAMPLE picks a sample from the distribution and returns its index.
%
% Input:
%   weights - array of sample weights
%
% Output:
%   idx - index of chosen sample

% use rand() for random number generation in open interval (0, 1)

weight_sum = 0;
random_number = rand() * sum(weights);
for i = 1:length(weights)
    weight_sum = weight_sum + weights(i);
    if random_number <= weight_sum
        idx = i;
        return
    end
end
