function res = check2dgauss(fname)
% CHECK2DGAUSS Checks if given file containes bivariate Gaussians.
%
% Synopsis:
%  res = check2dgauss(fname)
%
% Description:
%  If the file 'fname' contains a matrix Mean [2 x ncomp],
%  a matrix Cov [2 x 2 x ncomp] and a vector y [1 x ncomp]
%  then it returns 1 else it returns  0.
%
% Input:
%  fname [string] File name.
% 
% Output:
%  res [1x1] Equals to 1 if it is valid file otherwise 0.
%

% About: Statistical Pattern Recognition Toolbox                                
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac                     
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>            
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>       
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>           
                                                                                
% Modifications: 
% 1-may-2004, VF
% 13-Feb-2003, VF  

data=load( fname);

if ~isfield( data, 'Mean'), res=0; return; end
if ~isfield( data, 'Cov'), res=0; return; end
if ~isfield( data, 'y'), res=0; return; end

if size(data.Mean,1) ~= 2, res=0; return; end
if size(data.Cov,1) ~= 2, res=0; return; end
if size(data.Cov,2) ~= 2, res=0; return; end
if size(data.Cov,3) ~= size(data.Mean,2), res=0; return; end
if size(data.y,2) ~= size(data.Mean,2), res=0; return; end

res=1;

return;
