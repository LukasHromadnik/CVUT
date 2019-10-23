function [data,model] = genlsdata( dim, num_data, margin )
% GENLSDATA Generates linearly separable binary data.
%
% Synopsis:
%  data = genlsdata(dim,num_data,margin)
%
% Description:
%  It generates randomly binary labeled vectors which 
%  are linearly separable with prescribed margin. 
% 
% Input:
%  dim [1x1] Data dimension.
%  num_data [1x1] Number of generated data.
%  margin [1x1] Minimal ensured margin (distance of the closest
%   vector to the separating hyperplane).
%
% Output:
%  data [struct] Generated data:
%   .X [dim x num_data] Sample data.
%   .y [1 x num_data] Data labels (1 or 2).
%
%  model [struct] Ground truth linear classifier:
%   .W [dim x 1] Normal vector of separating hyperplane.
%   .b [1x1] Bias of the hyperplane.
%
% Example:
%  data = genlsdata(2,50,1);
%  model = ekozinec( data );
%  model.margin
%  figure; ppatterns(data); pline(model);
%
% See also 
%  PERCEPTRON, EKOZINEC, LINCLASS, SVM.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 3-may-2004, VF
% 16-Feb-2003, VF
% 26-feb-2001 V.Franc

data.X = 2*rand(dim, num_data )-1;
if nargin == 3,
  data.X = data.X*margin*10;
end

model.W = 2*rand(dim,1)-1;
model.b = 0;
data.y = linclass(data.X,model);
model.b = rand(1);
data.X = data.X - model.b/sum(model.W);

if nargin ==3,

  inx1 = find(data.y == 1);
  proj1 = (model.W'*data.X(:,inx1) + model.b)/norm(model.W);
  shift1 = max([ones(1,length(inx1))*margin ; proj1]) - proj1;
  data.X(:,inx1) = data.X(:,inx1) + ...
      (model.W*ones(1,length(inx1))/norm(model.W)).*(ones(dim,1)*shift1);

  inx2 = find(data.y == 2);
  proj2 = -(model.W'*data.X(:,inx2) + model.b)/norm(model.W);
  shift2 = max([ones(1,length(inx2))*margin ; proj2]) - proj2;
  data.X(:,inx2) = data.X(:,inx2) - ...
      (model.W*ones(1,length(inx2))/norm(model.W)).*(ones(dim,1)*shift2);
end

return;

