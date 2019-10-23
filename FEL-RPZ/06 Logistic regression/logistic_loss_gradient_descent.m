function [w, wt, Et] = logistic_loss_gradient_descent(X, y, w_init, epsilon)
% [w, wt, Et] = logistic_loss_gradient_descent(X, y, w_init, epsilon)
%
%   Performs gradient descent optimization of the logistic loss function.
%
%   Parameters:
%       X - d-dimensional observations of size [d, number_of_observations]
%       y - labels of the observations of size [1, number_of_observations]
%       w_init - initial weights of size [d, 1]
%       epsilon - parameter of termination condition: norm(w_new - w_prev) <= epsilon
%
%   Return:
%       w - resulting weights
%       wt - progress of weights (of size [d, number_of_iterations])
%       Et - progress of logistic loss (of size [1, number_of_iterations])

w = w_init;
step_size = 1;
E = logistic_loss(X, y, w);
g = logistic_loss_gradient(X, y, w);

wt = [];
wt(:, end + 1) = w;
Et = [E];

while 1
    w_new = w - step_size * g;
    E_new = logistic_loss(X, y, w_new);
    g_new = logistic_loss_gradient(X, y, w_new);
    
    if E_new < E
        % Success
        wt(:, end + 1) = w_new;
        Et(end + 1) = E_new;
        w = w_new;
        g = g_new;
        E = E_new;
        step_size = step_size * 2;
        
        if norm(wt(:, end - 1) - wt(:, end)) < epsilon
            break;
        end
    else
        step_size = step_size / 2;
    end
    
end
