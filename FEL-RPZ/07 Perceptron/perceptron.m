function [w, b] = perceptron(X, y, maxIterations)
    
% Implements the perceptron algorithm
% (http://en.wikipedia.org/wiki/Perceptron)
%   
%   Parameters:
%       X - training samples (DxN matrix)
%       y - training labels (1xN vector, contains either 1 or 2)
%
%   Returns:
%       w - weights vector (Dx1 vector)
%       b - bias (offset) term (1 double)

v = zeros(1, size(X, 1) + 1);
z = [X' ones(size(X, 2), 1)];

for i = 1:size(y, 2)
    if y(i) == 2
        z(i,:) = z(i,:) .* (-1);
    end
end

for iteration = 1:maxIterations
    found = 0;
    for element = 1:size(X, 2)
        if dot(v, z(element,:)) <= 0
            v = v + z(element,:);
            found = 1;
            break;
        end
    end
    
    if found == 0
        break;
    end
    
    if (found == 1) && (iteration == maxIterations)
        v = [NaN NaN];
    end
end

w = v(1:end-1)';
b = v(end);
    
end