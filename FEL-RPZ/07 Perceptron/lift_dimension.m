function Z = lift_dimension(X)
    
% Lifts the dimensionality of the feature space from 2 to 5 dimensions
%
%   Parameters:
%       X - training samples in the original space (2xN matrix)
%
%   Returns:
%       Z - training samples in the lifted feature space (5xN vector)


first = X(1,:);
second = X(2,:);

Z = [first; second; first.^2; first .* second; second.^2];

end