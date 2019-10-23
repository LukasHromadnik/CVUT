function label = classify_2normal(imgs, q)
% label = classify_2normal(imgs, q)
%
%   Classify images using continuous measurement and strategy q.
%
%   Parameters:
%       images - test set images, <h x w x n>
%       q - strategy
%               q.t1 q.t2 - two descision thresholds 
%               q.decision - 3 decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
%                            shape <1 x 3>
%
%   Return:
%       label - image labels, <1 x n>

x = compute_measurement_lr_cont(imgs);
label = ones(1,length(x));
for index = 1:length(x)
    if x(index) < q.t1
        label(index) = q.decision(1);
    elseif x(index) < q.t2
        label(index) = q.decision(2);
    else
        label(index) = q.decision(3);
    end
end
