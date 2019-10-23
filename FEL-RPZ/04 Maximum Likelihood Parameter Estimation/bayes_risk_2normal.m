function R = bayes_risk_2normal(distribution1, distribution2, q)
% R = bayes_risk_2normal(distribution1, distribution2, q)
%
%   Compute bayesian risk of a strategy q for 2 normal distributions and zero-one loss function.
%
%   Parameters:
%       distribution1 - parameters of the normal dist. distribution1.Mean, distribution1.Sigma
%       distribution2 - the same as distribution1
%       q - strategy
%               q.t1 q.t2 - two descision thresholds 
%               q.decision - 3 decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
%                            shape <1 x 3>
%
%   Return:
%       R - bayesian risk, scalar

R = 0;

for index = 1:length(q.decision)
    decision = q.decision(index);
    
    if index == 1
        t1 = -Inf;
        t2 = q.t1;
    elseif index == 2
        t1 = q.t1;
        t2 = q.t2;
    else
        t1 = q.t2;
        t2 = Inf;
    end
    
    if decision == 1
        sigma = distribution1.Sigma;
        mean = distribution1.Mean;
        prior = distribution1.Prior;
    else
        sigma = distribution2.Sigma;
        mean = distribution2.Mean;
        prior = distribution2.Prior;
    end
    
    fun = @(x) (1 / (sqrt(2 * pi) * sigma)) * exp(-1 * (((x - mean).^2)/(2 * sigma^2)));
    sum = prior * integral(fun, t1, t2);
    if ~(isnan(sum))
        R = R + sum;
    end
end

R = 1 - R;
