function out=c2s(in)
% C2S Converts cell to structure array.
%
% Synopsis:
%  out = c2s({'item1',value1,'item2',value2,...})
%
% Description:
%  out = c2s({'item1',value1,'item2',value2,...}) is a shortcut 
%   for out = struct('item1',value1,'item2',value2,...).
%
% Example:
%  out = c2s({'a',1,'b',2,'c','Hello'})
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 09-jun-2004, VF
% 05-may-2003, VF

if iscell(in),
  out = struct(in{:});
else
  out = in;
end

return;
% EOF