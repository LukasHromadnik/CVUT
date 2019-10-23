function risk = risk_optimal_strategy_prior_discrete(prob1, prob2, priors1, W)

for index = 1:length(priors1)
    prior1 = priors1(index);
    
    distribution1.Prob = prob1;
    distribution1.Prior = prior1;
    
    distribution2.Prob = prob2;
    distribution2.Prior = 1 - prior1;
    
    q = find_strategy_discrete(distribution1, distribution2, W);
    risk(index) = bayes_risk_discrete(distribution1, distribution2, W, q);
end
