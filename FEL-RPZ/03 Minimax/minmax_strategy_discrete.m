function [q, risk] = minmax_strategy_discrete(D1, D2)
% [q risk] = minmax_strategy_cont(D1, D2)
%
%   Find minmax strategy.
%
%   Parameters:
%       D1, D2 - 2 discrete distributions
%                D1.Prob - <1x21> vector of conditional probs
%                prior not needed
%
%   Returns:
%       q - strategy, <1x21> vector of 1 and 2 (see find_strategy_discrete)
%       risk - bayes risk of the minimax strategy q (in discrete
%       case, use the worse case risk of the optimal strategy - do
%       you know why?)

W = [0 1; 1 0];
found_prior = fminbnd(@(x) max_risk_strategy(x), 0, 1);
D1.Prior = found_prior;
D2.Prior = 1 - found_prior;
q = find_strategy_discrete(D1, D2, W);
risk = bayes_risk_discrete(D1, D2, W, q);

function max_risk = max_risk_strategy(prior)
    max_risk = -1;
    D1.Prior = prior;
    D2.Prior = 1 - prior;
    q = find_strategy_discrete(D1, D2, W);
    for my_prior = 0:0.01:1
        D1.Prior = my_prior;
        D2.Prior = 1 - my_prior;
        current_risk = bayes_risk_discrete(D1, D2, W, q);
        if current_risk > max_risk
            max_risk = current_risk;
        end
    end
end

end