function [q, risk] = minmax_strategy_cont(D1, D2)
% [q risk] = minmax_strategy_cont(D1, D2)
%
%   Find minmax strategy.
%
%   Parameters:
%       D1, D2 - 2 normal distributions
%                D1.Sigma, D1.Mean
%                prior not needed
%
%   Returns:
%       q - strategy (see find_strategy_2normal)
%       risk - bayes risk of the minimax strategy q

found_prior = fminbnd(@(x) max_risk_strategy(x), 0, 1);
D1.Prior = found_prior;
D2.Prior = 1 - found_prior;
q = find_strategy_2normal(D1, D2);
risk = bayes_risk_2normal(D1, D2, q);

function max_risk = max_risk_strategy(prior)
    D1.Prior = prior;
    D2.Prior = 1 - prior;
    q = find_strategy_2normal(D1, D2);
    max_risk = -1;
    for prior = 0:0.01:1
        D1.Prior = prior;
        D2.Prior = 1 - prior;
        risk = bayes_risk_2normal(D1, D2, q);
        if risk > max_risk
            max_risk = risk;
        end
    end
end

end