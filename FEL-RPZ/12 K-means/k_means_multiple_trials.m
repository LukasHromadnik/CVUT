function [c, means, sq_dists] = k_means_multiple_trials(x, k, n_trials, max_iter, show)
% [c, means, sq_dists] = k_means_multiple_trials(x, k, n_trials, max_iter, show)
%
% Performs several trials of the k-means clustering algorithm in order to
% avoid local minima. Result of the trial with the lowest "within-cluster
% sum of squares" is selected as the best one and returned.
%
% Input:
%   x         .. Feature vectors, of size [dim,number_of_vectors], where dim
%                is arbitrary feature vector dimension.
%
%   k         .. Required number of clusters (single number).
%
%   n_trials  .. Number of trials.
%
%   max_iter  .. Stopping criterion: max. number of iterations (single number)
%                for each of the trials. Set it to Inf if you wish to deactivate
%                this criterion. 
%
%   show      .. Boolean switch to turn on/off visualization of partial results.
%
% Output (= information about the best clustering from all the trials):
%   c         .. Cluster index for each feature vector, of size
%                [1, number_of_vectors], containing only values from 1 to k,
%                i.e. c(i) is the index of a cluster which the vector x(:,i)
%                belongs to.
%
%   means     .. Cluster centers, of size [dim,k], i.e. means(:,i) is the
%                center of the i-th cluster.
%
%   sq_dists   .. Squared distance to the nearest mean for each feature vector,
%                 of size [1, number_of_vectors].

if nargin < 5
    show = false;
end
%n_trials = 1;
% Multiple trial of the k-means clustering algorithm
for i_trial = 1:n_trials
    
    [c_part, means_part, sq_dists_part] = k_means(x, k, max_iter, show);
    cs(i_trial, :) = c_part;
    meanss(:, :, i_trial) = means_part
    sq_distss(i_trial, :) = sq_dists_part;
end

[~, min_idx] = min(sum(sq_distss, 2));

c = cs(min_idx, :);
means = meanss(:, :, min_idx);
sq_dists = sq_distss(min_idx, :);



