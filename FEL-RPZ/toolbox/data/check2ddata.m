function res=check2ddata(fname)
% CHECK2DDATA Checks if given file containins labeled 2D vectors.
%
% Synopsis:
%  res = check2ddata(fname)
%
% Description:
%  If the file 'fname' contains matrix X [2 x num_data]
%  and a vector y [1 x num_data] then it returns 1
%  else it returns 0.
%
% Input:
%  fname [string] File name.
% 
% Output:
%  res [1x1] Equals to 1 if it is valid file of 2D data otherwise 0.
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

if ~isfield( data, 'X'), res=0; return; end
if ~isfield( data, 'y'), res=0; return; end

if size(data.X,1) ~= 2, res=0; return; end
if size(data.X,2) ~= length(data.y), res=0; return; end

res=1;
return;
