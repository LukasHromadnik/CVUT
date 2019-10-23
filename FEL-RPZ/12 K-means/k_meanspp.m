function centers = k_meanspp(x, k)
% KMEANSPP - perform k-means++ initialization for k-means clustering.
%
% Input:
%   X - [DIMS x N] N input vectors of dimenionality DIMS
%   K - [int] number of k-means centers
%
% Output:
%   CENTERS - [DIMS x K] K proposed centers for k-means initialization

% Number of vectors
N = size(x, 2);

centers(:, 1) = x(:, random_sample(ones(1, size(x, 2))));

for iteration = 1:k - 1
    x_dists = zeros(1, N);
    
    for i = 1:N
        dists = x(:, i) - centers;
        x_dists(i) = min(sum(dists.^2, 1));
    end
    
    x_dists = x_dists / sum(x_dists);
    idx = random_sample(x_dists);
    centers(:, iteration + 1) = x(:, idx);
end
