function prior = estimate_prior(idLabel, labelling)
% prior = estimate_prior(idLabel, labelling)
%
%   Estimates prior probability of a class.
%
%   Parameters:
%       idLabel - id of the selected class
%       labelling - <1 x n> vector of label ids
%
%   Returns:
%       prior - prior probability

number_of_items = size(labelling);
count = 0;

for i = 1:number_of_items(2)
    if (labelling(i) == idLabel)
        count = count + 1;
    end
end

prior = count / number_of_items(2);

end