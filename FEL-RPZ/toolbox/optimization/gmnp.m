function [x,fval,stat] = gmnp(H,f,options)
% GMNP Solves Generalized Minimal Norm Problem. 
%
% Synopsis:
%  [x,fval,stat] = gmnp(H,f)
%  [x,fval,stat] = gmnp(H,f,options)
%
% Description:
%  The Generalized Minimal Norm Problem to solve reads 
%
%    min 0.5*x'*H*x + f'*x  subject to: sum(x) = 1 and x >= 0.
%     x
%
%  H is symetric positive-definite matrix. The GMNP is a special
%  instance of the Quadratig Programming (QP) task. The GMNP
%  is solved by one of the following algorithms:
%    mdm  ... Mitchell-Demyanov-Malozemov
%    imdm  ... Improved Mitchell-Demyanov-Malozemov (default).
%    iimdm  ... Improved (version 2) Mitchell-Demyanov-Malozemov.
%    kozinec ... Kozinec algorithm.
%    keerthi ... Derived from NPA algorithm by Keerthi et al.
%    kowalczyk ... Based on Kowalczyk's maximal margin perceptron.
%
%  The optimization halts if one of the following stopping 
%  conditions is satisfied:
%
%    options.tolabs >= UB-LB          -> exitflag = 1
%    options.tolrel*abs(UB) >= UB-LB  -> exitflag = 2
%    options.thlb < LB                -> exitflag = 3
%    options.tmax <= t                -> exitflag = 0
% 
%  where t is number of iterations, UB and LB are upper and 
%  lower bounds on the optimal solution.
%
%  For more info refer to V.Franc: Optimization Algorithms for Kernel 
%  Methods. Research report. CTU-CMP-2005-22. CTU FEL Prague. 2005.
%  ftp://cmp.felk.cvut.cz/pub/cmp/articles/franc/Franc-PhD.pdf .
%
% Input:
%  H [dim x dim] Symetric positiove definite matrix.
%  f [dim x 1] Vector.
%  options [struct] Control parameters:
%   .solver [string] Solver to be used: 'mdm', 'imdm' (default), 'iimdm', 
%     'kozinec', 'kowalczyk','keerthi'.
%   .tmax [1x1] Maximal number of iterations (default inf).
%   .tolabs [1x1] Absolute tolerance stopping condition (default 0.0).
%   .tolrel [1x1] Relative tolerance stopping condition (default 1e-6).
%   .thlb [1x1] Thereshold on the lower bound (default inf).
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
%  X = rand(50,200); f = rand(200,1); e = ones(200,1);
%  tic; [x1,fval1] = gmnp(X'*X,f); toc, fval1
%  tic; [x2,fval2] = quadprog(X'*X,f,[],[],e',1,e*0); toc, fval2
%
% See also 
%  GNNLS, RSDE, BSVM2.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2005, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 09-sep-2005, VF
% 24-jan-2005, VF
% 20-nov-2004, VF
% 16-jul-2004, VF
% 29-jun-2004, VF

% cputimer
tic;

% options
if nargin < 3, options = []; else options = c2s(options); end
if ~isfield(options,'tolabs'), options.tolabs = 0; end
if ~isfield(options,'tolrel'), options.tolrel = 1e-6; end
if ~isfield(options,'thlb'), options.thlb = inf; end
if ~isfield(options,'tmax'), options.tmax = inf; end
if ~isfield(options,'verb'), options.verb = 0; end
if ~isfield(options,'solver'), options.solver = 'imdm'; end

[x,exitflag,t,access,History] = gmnp_mex( H, f, ...
   options.solver,options.tmax,options.tolabs,options.tolrel,...
   options.thlb, options.verb );

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