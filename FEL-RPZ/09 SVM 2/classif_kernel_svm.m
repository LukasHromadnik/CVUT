function [ classif ] = classif_kernel_svm( X, model )
%CLASSIF_KERNEL_SVM performs classification on X by trained SVM classifier stored in model
% 
% Input:
% X         [n x m (double)] matrix containing feature points in columns
% model     [1 x 1 (struct)] structure with fields (.b, .fun, .sv, .y, .alpha, .options see my_kernel_svm.m for details)
% 
% Output:
% classif   [1 x m (double)] vector with labels for X obtained by classification 
% 

s = 0;
for i = 1:size(model.sv, 2)
    kernel = getKernel(X, model.sv(:,i), model.options);
    s = s + model.alpha(i) * model.y(i) * kernel;
end
s = s + model.b;

classif = sign(s)';

end

