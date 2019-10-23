function model = fldqp(data)
% FLDQP Fisher Linear Discriminat using Quadratic Programming.
%
% Synopsis:
%  model = fldqp( data )
%
% Description:
%  This function computes the binary linear classifier based
%  on the Fisher Linear Discriminant (FLD) using the Quadratic
%  Programming (quadprog) optimization. The inputs are
%  binary labeled training vectors. The parameter vector W
%  of the linear classifier
%    q(x) = 1  for W'*x + b >= 0
%         = 2  for W'*x + b < 0
%  
%  is computed to maximize class separability criterion.
%  The bias b is determined to lie between means of training
%  data projected onto direction W. 
%
% Input:
%  data [struct] Binary labeled training vectors.
%   .X [dim x num_data] Training vectors.
%   .y [1 x num_data] Labels (1 or 2).
%
% Output:
%  model [struct] Binary linear classifier:
%   .W [dim x 1] Parameter vector the linear classifier.
%   .b [1x1] Bias of the linear classifier.
%   .separab [1x1] Meassure of class separability.
%
% Example:
%  trn = load('riply_trn');
%  tst = load('riply_tst');
%  model = fldqp( trn );
%  ypred = linclass( tst.X, model);
%  cerror(ypred, tst.y)
%  figure; ppatterns(trn); pline(model);
%
% See also 
%  FLD, LINCLASS.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 21-may-2004, VF
% 1-may-2004, VF
% 30-apr-2004, VF
% 24-Feb-2003, VF
% 1-Feb-2003, VF

% inputs
%-------------------------------
[dim,num_data ] = size(data.X );

% compute means and scatter matrix
%-------------------------------
inx1 = find( data.y == 1);
inx2 = find( data.y == 2);
n1 = length(inx1);
n2 = length(inx2);

m1 = mean(data.X(:,inx1),2);
m2 = mean(data.X(:,inx2),2);

S1 = (data.X(:,inx1)-m1*ones(1,n1))*(data.X(:,inx1)-m1*ones(1,n1))';
S2 = (data.X(:,inx2)-m2*ones(1,n2))*(data.X(:,inx2)-m2*ones(1,n2))';
Sw = S1 + S2;

% set params for QP
%------------------------
f=zeros(dim,1);
A=-(m1-m2)';
b=-2;

[W,rinv]=quadprog(Sw,f,[],[],A,b,...
       [],[],[],optimset('Display','off'));

proj_m1 = W'*m1;
proj_m2 = W'*m2;

model.W = W;
model.b = -0.5*(proj_m1+proj_m2);
model.separab = (proj_m1-proj_m2)^2/(W'*Sw*W);
model.fun = 'linclass';

return;

