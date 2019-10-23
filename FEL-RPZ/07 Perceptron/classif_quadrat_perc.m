function K = classif_quadrat_perc(tst, model)

% Classifies test samples using the quadratic discriminative function 
%
%   Parameters:
%       tst - samples for classification in original space (2xN matrix)
%       model - structure with the trained perceptron classifier
%       (parameters of the descriminative function)
%           model.w - weights vector (5x1 vector)
%           model.b - bias term (1 double)
%
%   Returns:
%       K - classification result (1xN vector, contains either 1 or 2)

Z = lift_dimension(tst);

for i = 1:size(Z, 2)
    if dot(Z(:,i), model.w) + model.b < 0
        K(i) = 2;
    else
        K(i) = 1;
    end
end

end
