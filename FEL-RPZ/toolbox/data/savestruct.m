function savestruct(structure,file_name)
% SAVESTRUCT saves fields of given structure to file.
%
% Synopsis:
%   savestruct(structure)
%   savestruct(structure,file_name)
%
% Description:
%  It saves fields of given structure to specified file.
%
% Input:
%  structure [struct] Input structure. 
%  file_name [string] File name (default 'struct.mat'.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 21-apr-2004, VF

if nargin < 2, file_name = 'struct.mat'; end

names = fieldnames(structure);

save_str = 'save(file_name';
for i=1:length(names);
  save_str = [save_str ', ''' names{i} ''''];
  
  eval([names{i} '= structure.' names{i} ';']);
  
end

save_str = [save_str ');'];

eval(save_str);

return;

%EOF