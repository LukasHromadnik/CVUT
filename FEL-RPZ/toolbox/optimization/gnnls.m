function [x,fval,stat] = gnnls(H,f,options)
% GNNLS Solves Generalized Non-negative Least Squares (GNNLS) problem. 
%
% Synopsis:
%  [x,fval,stat] = gnnls(H,f)
%  [x,fval,stat] = gnnls(H,f,options)
%
% Description:
%  The Generalized Non-negative Least Squares to solve reads 
%
%   argmin 0.5*x'*H*x + f'*x  subject to: x(i) >= 0 for all i.
%     x
%
%  H is symetric positive semi-definite matrix and f a vector. 
%  The GNNLS is a special instance of the Quadratig Programming 
%  (QP) task which can be solved by the Sequential Coordinate-wise 
%  Algorithm (Franc, Hlavac: Sequential Coordinate-wise Algorithm
%  for Non-negative Least Squares Problem. Research report. 
%  CTU-CMP-2005-06. CTU FEL Prague. 
%  ftp://cmp.felk.cvut.cz/pub/cmp/articles/franc/Franc-TR-2005-06.ps).
%
%  The optimization halts if one of the following stopping 
%  conditions is satisfied:
%
%    t >= options.tmax           ->  exit_flag = 0  Number of iterations.
%    UB-LB <= options.tolabs     ->  exit_flag = 1  Abs. tolerance.
%    UB-LB <= UB*options.tolrel  ->  exit_flag = 2  Relative tolerance.
%    Relaxed KKT cond. satisfied ->  exit_flag = 3  It means that
%       mu(i) >= -options.tolKKT  for all i &&  
%       mu(i) <= options.tolKKT   for i such that x(i) > 0
%       where mu = H*x + f                        
%
%  where t is number of iterations, UB and LB are upper and 
%  lower bounds on the optimal solution. The stopping conditions 
%  using UB and LB can be applied only if the matric H has all elements 
%  non-negative.
%
% Input:
%  H [dim x dim] Symetric positive definite matrix.
%  f [dim x 1] Vector.
%  options [struct] Control parameters:
%   .tmax [1x1] Maximal number of iterations (default inf).
%   .tolabs [1x1] Absolute tolerance stopping condition (default 0.0).
%   .tolrel [1x1] Relative tolerance stopping condition (default 1e-6).
%   .tolKKT [1x1] Tolerance of satisfaction of KKT conditions (default 1e-6).
%   .verb [1x1] If > 0 then displays info every verb-th iteration (default 0).
%
% Output:
%  x [dim x 1] Solution vector.
%  fval [1x1] Value of the criterion at x.
%  stat [struct] Constains:
%   .exiflag [1x1] Exitflag (see above).
%   .t [1x1] Number of iterations.
%   .LB_History [1 x t] History of lower bounds.
%   .UB_History [1 x t] History of upper bounds.
%   .access [1x1] Number of accesses to the matrix H.
%   .LB [1x1] == LB_History(end).
%   .UB [1x1] == UB_History(end).
%   .cputime [1x1] CPU time meassured by tic toc functions.
%
% Example:
%  A = rand(1000,250); b = rand(1000,1); 
%  tic; x1 = lsqnonneg(A,b); toc, F1 = norm(A*x1-b)^2
%  tic; x2 = gnnls(A'*A, -A'*b); toc, F2 = norm(A*x2-b)^2
%
% See also 
%  GMNP.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2005, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 09-sep-2005, VF
% 28-jan-2005, VF

% cputimer
tic;

% options
if nargin < 3, options = []; else options = c2s(options); end
if ~isfield(options,'tolabs'), options.tolabs = 0; end
if ~isfield(options,'tolrel'), options.tolrel = 0; end
if ~isfield(options,'tolKKT'), options.tolKKT = 1e-6; end
if ~isfield(options,'tmax'), options.tmax = inf; end
if ~isfield(options,'verb'), options.verb = 0; end
if ~isfield(options,'solver'), options.solver = 'sca'; end


[x,exitflag,t,access,History] = gnnls_mex(H,f,options.solver,...
   options.tmax,options.tolabs,options.tolrel,options.tolKKT,options.verb);

stat.t = t;
stat.exitflag = exitflag;
stat.LB_History = History(1,:);
stat.UB_History = History(2,:);
stat.LB = History(1,end);
stat.UB = History(2,end);
stat.access = access;
stat.cputime = toc;

fval = stat.UB;

return;
% EOF