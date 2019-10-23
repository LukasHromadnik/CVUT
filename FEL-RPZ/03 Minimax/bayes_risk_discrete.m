function R = bayes_risk_discrete(distribution1, distribution2, W, q)
% R = bayes_risk_discrete(distribution1, distribution2, W, q)
%
%   Compute bayesian risk for a discrete strategy q
%
%   Parameters:
%           distribution1.Prob      pXk(x|A) given as a <1 × n> vector
%           distribution1.Prior 	prior probability pK(A)
%           W                       cost function matrix
%                                   dims: <states x decisions>
%                                   (nr. of states and decisions is fixed to 2)
%           q                       strategy - <1 × n> vector, values 1 or 2
%
%   Return:
%           R - bayesian risk, <1 x 1>
sum = 0;
for index = 1:length(q)
    sum = sum + distribution1.Prob(index)*distribution1.Prior*W(1,q(index));
    sum = sum + distribution2.Prob(index)*distribution2.Prior*W(2,q(index));
end
R = sum;

