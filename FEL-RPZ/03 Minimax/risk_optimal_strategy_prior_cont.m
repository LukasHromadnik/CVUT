function risk = risk_optimal_strategy_prior_cont(dist1, dist2, priors1)

risk = zeros(size(priors1));

for index = 1:length(priors1)
    prior1 = priors1(index);
    
    dist1.Prior = prior1;
    dist2.Prior = 1 - prior1;
    
    q = find_strategy_2normal(dist1, dist2);
    risk(index) = bayes_risk_2normal(dist1, dist2, q);
end
