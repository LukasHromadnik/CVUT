function risks = risk_fix_q_discrete(D1, D2, D1_priors, q )
% risks = risk_fix_q_discrete(D1, D2, D1_priors, q )
%
%   Computes risk(s) for varying prior.
%
%   Parameters:
%       D1, D2 - discrete distributions, priors not needed
%       D1_priors <1xn> vector of D1 priors
%       q - strategy
%
%   Returns:
%       risks - <1xn> vector of bayesian risk of the strategy q 
%               with 0-1 cost function and varying priors D1_priors

W = [0 1; 1 0];

risks = zeros(size(D1_priors));

for index = 1:length(D1_priors)
    prior1 = D1_priors(index);
    D1.Prior = prior1;
    D2.Prior = 1 - prior1;
    risks(index) = bayes_risk_discrete(D1, D2, W, q);
end
