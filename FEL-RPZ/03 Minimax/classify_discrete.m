function labels = classify_discrete(img, q)
% label = classify_discrete(imgs, q)
%
%   Classify images using discrete measurement and strategy q.
%
%   Parameters:
%       images - test set images, <h x w x n>
%       q - strategy <1 × 21> vector of 1 or 2
%
%   Return:
%       label - image labels, <1 x n>
x = compute_measurement_lr_discrete(img);
labels = ones(1,length(x));
for index = 1:length(x)
    labels(index) = q(x(index) + 11);
end
