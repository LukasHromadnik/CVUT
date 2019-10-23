function red_model = rsrbf(model,options)
% RSRBF Redused Set Method for RBF kernel expansion.
%
% Synopsis:
%  red_model = rsrbf(model)
%  red_model = rsrbf(model,options)
%
% Description:
%  red_model = rsrbf(model) searchs for a kernel expansion
%    with nsv vectors which best approximates the input 
%    expansion given in model [Schol98a]. The Radial Basis 
%    kernel (RBF) is assumed (see 'help kernel').
%    
%  red_model = rsrbf(model,options) allows to specify the 
%    control paramaters.
%
% Input:
%  model [struct] Kernel expansion:
%   .Alpha [nsv x 1] Weight vector.
%   .sv.X [dim x nsv] Vectors defining the expansion.
%   .options.ker [string] Must be equal to 'rbf'.
%   .options.arg [1x1] Kernel argument (see 'help kernel').
% 
%  options [struct] Control parameters:
%   .nsv [1x1] Desired number of vectors in the reduced 
%     expansion (default round(length(model.Alpha)/2)).
%   .eps [1x1] Desier limit on the norm of difference between 
%     the original  normal vector and the reduced the normal 
%     vector in the  feature space. The algorithm is stopped 
%     when a lower  difference is achived (default 1e-6).
%   .preimage [string] Function called to solve the RBF pre-image 
%     problem (default 'rbfpreimg');
%   .verb [1x1] If 1 then progress info is display (default 0).
% 
% Output:
%  red_model [struct] Reduced kernel expansion.
%
% Example:
%  trn = load('riply_trn');
%  model = smo(trn,struct('ker','rbf','arg',1,'C',10));
%  red_model = rsrbf(model,struct('nsv',10));
%  figure; ppatterns(trn);
%  h1 = pboundary(model,struct('line_style','r'));
%  h2 = pboundary(red_model,struct('line_style','b'));
%  legend([h1(1) h2(1)],'Original SVM','Reduced SVM');
%
% See also 
%  RBFPREIMG.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 11-oct-2004, VF, knorm.m used 
% 21-sep-2004, VF
% 10-jun-2004, VF
% 02-dec-2003, VF

% process inputs
%--------------------------------------------------
if nargin < 2, options=[]; else options = c2s(options); end
if ~isfield(options,'nsv'), options.nsv = round(length(model.Alpha)/2); end
if ~isfield(options,'eps'), options.eps = 1e-6; end
if ~isfield(options,'preimage'), options.preimage = 'rbfpreimg2'; end
if ~isfield(options,'verb'), options.verb = 0; end

% init
%--------------------------------------------------
Z=[];
Beta = [];
Alpha = model.Alpha(:);
X = model.sv.X;
%Const = Alpha'*kernel(X, model.options.ker, model.options.arg )*Alpha;
Const = knorm(X,Alpha,model.options.ker,model.options.arg)^2;
error = inf;
iter = 0;

% main loop
%--------------------------------------------------
while error > options.eps & iter < options.nsv,
  
  iter = iter + 1;

  if options.verb, 
    fprintf('Iteration %d: ', iter);
  end
  
  tmp_model.Alpha = Alpha;
  tmp_model.sv.X = X;
  tmp_model.options = model.options;

  if options.verb, 
     fprintf('computing preimage, ');
  end
%  Z = [Z, rbfpreimg2(tmp_model)];
  Z = [Z, feval( options.preimage,tmp_model)];
  
  Kz = kernel( Z, model.options.ker, model.options.arg );
  Kzs = kernel( Z, model.sv.X, model.options.ker, model.options.arg );
  Beta = inv( Kz ) * Kzs * model.Alpha(:);
  
  error = Const + Beta'*Kz*Beta - 2*Beta'*Kzs*model.Alpha(:);
  if options.verb, 
     fprintf('||w-w_red||^2 = %f\n', error);
  end

  Alpha = [model.Alpha(:); -Beta(:)]';
  X = [model.sv.X, Z];
  
end

red_model = model;
red_model.Alpha = Beta;
red_model.sv.X = Z;
red_model.nsv = size(Z,2);

return;
%EOF