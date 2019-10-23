function worst_risks = worst_risk_discrete(D1, D2, D1_priors)
% worst_risks = worst_risk_discrete(D1, D2, D1_priors)
%
%   Compute worst possible risks of a bayesian strategies.
%
%   Parameters:
%       D1, D2 - discrete distributions
%       D1_priors - <1 x n> vector of D1 priors to be used
%
%   Returns:
%       worst_risks - <1 x n> worst risk of bayesian strategies
%                     for D1, D2 with different priors D1_priors


%   Hint: for all D1_priors calculate bayesian strategy and 
%   corresponding maximal risk.

W = [0 1; 1 0];
worst_risks = zeros(size(D1_priors));

for index = 1:length(D1_priors)
    prior1 = D1_priors(index);
    D1.Prior = prior1;
    D2.Prior = 1 - prior1;
    q = find_strategy_discrete(D1, D2, W);
    max_risk = -1;
    for prior = D1_priors
        D1.Prior = prior;
        D2.Prior = 1 - prior;
        risk = bayes_risk_discrete(D1, D2, W, q);
        if risk > max_risk
            max_risk = risk;
        end
    end
    worst_risks(index) = max_risk;
end
