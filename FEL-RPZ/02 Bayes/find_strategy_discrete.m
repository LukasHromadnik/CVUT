function q = find_strategy_discrete(distribution1, distribution2, W)
% q = find_strategy_discrete(distribution1, distribution2, W)
%
%   Find bayesian strategy for 2 discrete distributions.
%   
%   Parameters:
%       distribution1.Prob      pXk(x|A) given as a <1 × n> vector
%       distribution1.Prior 	prior probability pK(A)
%       distribution2.Prob      ...
%       distribution2.Prior 	...
%       W - cost function matrix, <states x decisions>
%                                (nr. of states is fixed to 2)
%
%   Return: 
%       q - optimal strategy <1 x n>

q = ones(1,length(distribution1.Prob));
for index = 1:length(distribution1.Prob)
    posterior1 = distribution1.Prob(index) * distribution1.Prior;
    posterior2 = distribution2.Prob(index) * distribution2.Prior;
    
    a = posterior1 * W(1,1) + posterior2 * W(2,1);
    c = posterior1 * W(1,2) + posterior2 * W(2,2);
    
    if c < a
        q(index) = 2;
    end
end
