function [var_mean, var_sigma] = mle_variance(cardinality)
% [var_mean var_sigma] = mle_variance(cardinality)
%
%   Estimates variance of estimated parameters of a normal distribution 
%   in 100 trials.
%
%   Parameters:
%       cardinality - size of the generated dataset (e.g. 1000)
%   Returns
%       var_mean - variance of the estimated means in 100 trials
%       var_sigma - variance of the estimated standard deviation in 100 trials

 
numTrials = 100;
muRec = zeros(1,numTrials);
sigmaRec = zeros(1,numTrials);

for i = 1:numTrials
    test = zeros(1, cardinality);
    
    for j = 1:cardinality
        test(j) = randn;
    end
    
    [mu, sigma] = mle_normal(test);
    muRec(i) = mu;
    sigmaRec(i) = sigma;
end

mu_muRec = sum(muRec) / numTrials;
mu_sigmaRec = sum(sigmaRec) / numTrials;

var_mean = sum((muRec - mu_muRec).^2) / numTrials;
var_sigma = sum((sigmaRec - mu_sigmaRec).^2) / numTrials;

end
