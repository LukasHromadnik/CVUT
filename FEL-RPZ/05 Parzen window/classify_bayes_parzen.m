function labels = classify_bayes_parzen(x_test, xA, xC, pA, pC, h_bestA, h_bestC)
% labels = classify_bayes_parzen(x_test, xA, xC, pA, pC, h_bestA, h_bestC)
%
%   Classifies data using bayesian classifier with densities estimated by
%   Parzen window estimator.
%
%   Parameters:
%       x_test - data (measurements) to be classified
%       xA, xC - training data for Parzen window for class A and C
%       pA, PC - prior probabilities
%       h_bestA, h_bestC - optimal values of the kernel bandwidth
%
%   Returns:
%       labels - classification labels for x_test

pxA = my_parzen(x_test, xA, h_bestA) * pA;
pxC = my_parzen(x_test, xC, h_bestC) * pC;

labels = ones(size(pxA));

for index = 1:length(pxA)
    if pxC(index) > pxA(index)
        labels(index) = 2;
    end
end
