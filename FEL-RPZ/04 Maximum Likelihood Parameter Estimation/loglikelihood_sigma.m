function [L, maximizerSigma, maxL] = loglikelihood_sigma(x, D, sigmas)
% [L maximizerSigma maxL] = loglikelihood_sigma(x, D, sigmas)
%
%   Compute log ligelihoods and maximum ll of a normal
%   distribution with fixed mean and variable standard deviation.
%
%   Parameters:
%       x - input features <1 x n>
%       D - D.Mean the normal distribution mean
%       sigmas - <1 x m> vector of standard deviations
%
%   Returns:
%       L - <1 x m> vector of log likelihoods
%       maximizerSigma - sigma for the maximal log likelihood
%       maxL - maximal log likelihood
  
% no need of for loops!!!

L = arrayfun(@(sigma) ll(x, D.Mean, sigma), sigmas);
maximizerSigma = fminbnd(@(sigma) -ll(x, D.Mean, sigma), sigmas(1), sigmas(end));
maxL = ll(x, D.Mean, maximizerSigma);

function result = ll(x, mu, sigma)
    result = sum(log(normpdf(x, mu, sigma)));
end

end