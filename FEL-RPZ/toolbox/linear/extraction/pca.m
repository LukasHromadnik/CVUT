function model = pca(X,arg1)
% PCA Principal Component Analysis.
%
% Synopsis:
%  model = pca(X)
%  model = pca(X,new_dim)
%  model = pca(X,var)
%
% Description:
%  It computes Principal Component Analysis, i.e., the
%  linear transform which makes data uncorrelated and
%  minize the reconstruction error.
%
%  model = pca(X,new_dim) use to specify explicitely output
%   dimesnion where new_dim >= 1.
%
%  model = pca(X,var) use to specify a portion of discarded
%   variance in data where 0 <= var < 1. The new_dim is 
%   selected be as small as possbile and to satisfy 
%     var >= MsErr(new_dim)/MaxMsErr 
%   
%   where MaxMsErr = sum(sum(X.^2)). 
%
% Input:
%  X [dim x num_data] training data stored as columns.
%
%  new_dim [1x1] Output dimension; new_dim > 1 (default new_dim = dim);
%  var [1x1] Portion of discarded variance in data.
%
% Ouputs:
%  model [struct] Linear projection:
%   .W [dim x new_dim] Projection matrix.
%   .b [new_dim x 1] Bias.
%  
%   .eigval [dim x 1] eigenvalues.
%   .mse [real] Mean square representation error.
%   .MsErr [dim x 1] Mean-square errors with respect to number 
%     of basis vectors; mse=MsErr(new_dim).
%   .mean_X [dim x 1] mean of training data.
%
% Example:
%  in_data = load('iris');
%  model = pca(in_data.X, 2)
%  out_data = linproj(in_data,model);
%  figure; ppatterns(out_data);
%
% See also 
%  LINPROJ, PCAREC, KPCA.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 20-may-2004, VF
% 20-june-2003, VF
% 21-jan-03, VF
% 20-jan-03, VF
% 16-Jan-2003, VF, new comments.
% 26-jun-2002, VF

% get dimensions
[dim,num_data]=size(X);

% process input arguments
if nargin < 2, arg1 = 0; end

% centering data
mean_X = mean( X' )';
X=X-mean_X*ones(1,num_data);

% covariance
S = X*X';

% eigenvalue decomposition
[U,D,V]=svd(S);

model.eigval=diag(D)/num_data;
sum_eig = triu(ones(dim,dim),1)*model.eigval;
model.MsErr = sum_eig;

% decide about the new_dimension
if arg1 >= 1,
  new_dim = arg1;   % new_dim enterd explicitely
else
  inx = find( sum_eig/sum(model.eigval) <= arg1);
  if isempty(inx), new_dim = dim; else new_dim = inx(1); end
  model.var = arg1;
end

% take new_dim most important eigenvectors
model.W=V(:,1:new_dim);

% compute data translation
model.b = -model.W'*mean_X;
model.new_dim = new_dim;
model.mean_X = mean_X;
model.mse = model.MsErr(new_dim);
model.fun = 'linproj';

return;
