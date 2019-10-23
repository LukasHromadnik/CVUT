function [ TstErr ] = compute_kernel_TstErr( itrn, itst, X, y, C, options )
%COMPUTE_KERNEL_TSTERR Computes mean risk computed over crossvalidation folds (train svm on train set and evaluate on test set)
%
% Input:
% itrn      [1 x f (cell)] cell with f folds for crossvalidation (indices to X of training data)
% itst      [1 x f (cell)] cell with f folds for crossvalidation (indices to X of test data)
% X         [n x m (double)] matrix containing feature points in columns
% y         [1 x m (double)] vector with labels (-1, 1) for feature points in X
% C         [1 x 1 (double)] number with regularization constant C
% options   [1 x 1 (struct)] structure with parameters of kernel
% 
% Output:
% TstErr    [1 x 1 (double)] mean test error 

error = zeros(size(itrn, 2), 1);
for i = 1:size(itrn, 2)
    trn = cell2mat(itrn(i));
    tst = cell2mat(itst(i));
    [model] = my_kernel_svm(X(:,trn), y(trn), C, options);
    classification = classif_kernel_svm(X(:,tst), model);
    error(i) = sum(abs(classification - y(tst))) / 2;
end

TstErr = mean(error);

end
