function model=androrig(distrib,options,init_model)
% ANDRORIG Original method to solve the Anderson-Bahadur's task.
%
% Synopsis:
%  model = androrig(distrib)
%  model = androrig(distrib,options)
%  model = androrig(distrib,options,init_model)
%
% Description:
%  It solves the original Anderson task [Anderson62]. The goal is to 
%  find binary linear classifier which minimizes probability of 
%  misclassification. The class conditional probability distributions 
%  are Gaussians. The a prior probabilities is unknown.
%
%  model = androrig( distrib ) solves the original Anderson's task 
%    for given two Gaussians distributions. The structure distrib 
%    contains:
%     .Mean [dim x 2] Matrix containing mean vectors of the first and
%                     second class distributions.
%     .Cov  [dim x dim x 2]$ Matrix containing covariance matrices of the
%                     first and second distribution.
%
%  model = androrig( distrib, options ) allows to specify the maximal 
%    number of iterations options.tmax and the distance to the
%    optimal solution options.eps defining the stopping condition.
%
%  model = androrig( distrib, options, init_model ) allows to specify 
%    the initial point init_model.gamma. The initial value of the
%    counter of iterations can be specified in options.t.
%
% Input:
%  distrib [struct] Two Gaussians:
%   .Mean [ dim x 2] Mean veactors.
%   .Cov  [ dim x dim x 2] Covariance matrices.
%
%  options [struct] Defines stopping condition:
%   .tmax [1x1] Maximal number of iteration.
%   .eps [1x1] Closeness to the optimal solution. If eps=0 the
%     algorithm converges to the optimal solution but it does not
%     have to stop (default 0.001).
%
%  init_model [struct] Init model:
%   .gamma [1x1] Auxciliary variable (default 1).
%   .t [1x1] (optional) Counter of iterations.
%   
%
% Output:
%  model [struct] Binary linear classifier:
%   .W [dim x 1] Normal vector the found hyperplane W'*x+b=0.
%   .b [1x1] Bias of the hyperplane.
%  
%   .err [1x1] Probability of misclassification.
%   .t [1x1] Number of iterations.
%   .r1 [1x1] Mahalanobis distance of the first Gaussian to the
%     found hyperplane.
%   .r2 [1x1] Mahalanobis distance of the second Gaussian to the
%     found hyperplane. In the optimal solution r1 = r2.
%    .exitflag [1x1] 0 ... maximal number of iterations tmax exceeded.
%                    1 ... condition delta < eps satisfied.
%   .delta [1x1] Indicates distance from the optimal solution.
%   .gamma [1x1] Auxciliary variable.
%
% Example:
%  data = load('riply_trn');
%  distrib = mlcgmm(data);
%  model = androrig(distrib);
%  figure; pandr( model, distrib );
%
% See also 
%  GANDERS, EANDERS, GGRADANDR, LINCLASS.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 20-may-2004, VF
% 24-Feb-2003, VF

% process input arguments
% -----------------------------
if nargin < 1,
  error('Not enough input arguments.');
end

if nargin < 2, options = []; else options = c2s( options ); end
if ~isfield(options,'eps'), options.eps = 0.001; end
if ~isfield(options,'tmax'), options.tmax = inf; end

% get paramaters of distributions
if isfield(distrib, 'y' ) & (distrib.y(1) ~= distrib.y(2)),
  inx1 = 2; inx2 = 1;
else
  inx1 = 1; inx2 = 2;
end
m1=distrib.Mean(:,inx1);
m2=distrib.Mean(:,inx2);
dim=length(m1);
C1=distrib.Cov(:,:,inx1);
C2=distrib.Cov(:,:,inx2);

% Inicialization 
% -----------------------------------------
gamma=1;
delta=inf;
t=0;

if nargin == 3, ,
  gamma = init_model.gamma;
  if isfield( init_model, 't'), t = init_model.t; end
end

% Main loop cycle
%---------------------------------
while options.tmax > t & delta > options.eps,
   t=t+1;

   lambda=1/(1+gamma);

   W=inv((1-lambda)*C1+lambda*C2)*(m1-m2);
   gamma=sqrt((W'*C2*W)/(W'*C1*W));

   delta=abs( gamma - (1-lambda)/lambda );
end

b1 = -lambda*(W'*C2*W)-(W'*m2);
b2 = (1-lambda)*(W'*C1*W)-(W'*m1);
b = (b1+b2)/2;

r1 = (W'*m1 + b)/sqrt(W'*C1*W);
r2 = -(W'*m2 + b)/sqrt(W'*C2*W);
minr=min([r1 r2]);

% -- output model -----------------
model.W = W;
model.b=b;
model.err = 1-cdf('norm',minr,0,1);
model.t = t;
model.delta = delta;
model.gamma = gamma;
model.r1=r1;
model.r2=r2;
model.r=minr;

if delta <= options.eps, 
  model.exitflag = 1;
else
  model.exitflag = 0;
end

model.fun='linclass';

return;
