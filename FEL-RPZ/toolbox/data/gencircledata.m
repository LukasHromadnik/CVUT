function [X,gnd_X] = gencircledata(Center,R,num_data,dev)
% GENCIRCLEDATA Generates data on circle corrupted by Gaussian noise.
% 
% Synopsis:
%   [X,gnd_X] = gencircledata(Center,R,num_data,dev)
%
% Description:
%  The underlying curve is 2D circle given by Center and radius R.
%  This function randomly samples points on the circle and it adds 
%  Gaussian noise with given standard deviation dev.
%  
% Input:
%  Center [2x1] Circle center.
%  R [1x1] Circle radius.
%  num_data [1x1] Number of samples.
%  dev [1x1] Standard deviation of added Gaussian noise (default 1).
% 
% Output:
%  X [2xnum_data] Generated samples.
%  gnd_X [2xnum_data] Ground truth - samples without noise.
%
% Example:
%  [X,gnd_X] = gencircledata([1;1],5,500,1);
%  figure; 
%  ppatterns(X); ppatterns(gnd_X,'r+');
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 4-may-2004, VF

if nargin < 4, dev = 1; end

phi = 2*pi*rand(1,num_data);
gnd_X = repmat(Center,1,num_data)+R*[cos(phi);sin(phi)];
gnd_y = ones(1,num_data);

% add noise
X = gnd_X + randn(2,num_data)*dev;

return;
% EOF
