function mergesets( DirName, OutputFile )
% MERGESETS Merges data sets to one labeled data file.
%
% Synopsis:
%  mergesets(DirName,OutputFile)
%
% Description:
%  It loads data files from given directory DirName and
%  merges them to one labeled data file. The individual files
%  are assumed to contain the field X [dim x num_data].
%  The file names have to have the following format
%  name_xxx.mat ('_' can be omitted), where xxx denotes an 
%  integer which is used to label the data X from the given 
%  file. The OutputFile contains item X [dim x num_all_data] 
%  with all merged data and vector of labels y [1 x num_all_data]. 
% 
% Input:
%  DirName [string] Input directory.
%  OutputFile [string] Output data file.
%
% Output:
%  file OutputFile with items:
%   X [dim x num_all_data] Data matrix.
%   y [1 x num_all_data] Labels of data.
%

% (c) Statistical Pattern Recognition Toolbox, (C) 1999-2003,
% Written by Vojtech Franc and Vaclav Hlavac,
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>,
% <a href="http://www.feld.cvut.cz">Faculty of Electrical engineering</a>,
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
%  9-sep-2003, VF

list= dir(DirName);

X =[];
y = [];
for i=1:length(list),
   
   if ~list(i).isdir,
      
      num=GetLabel(list(i).name);
      if ~isempty(num),
         fprintf('Input file: %s, label: %d\n', list(i).name, num);
         
         data = load([DirName list(i).name]);
         X = [X , data.X];
         y = [y, ones(1,size(data.X,2))*num];
      end
   end
end

fprintf('Saving data to: %s\n', OutputFile );
save(OutputFile, 'X','y');

return;


function num=GetLabel( fname )

  if strcmpi( fname(end-2:end), 'mat' )~=1, 
     num=[]; 
     return;
  end

  for i=1:length(fname)-4,
     b = fname(i:end-4);
     num=str2num( b );
     if ~isempty(num), return; end
  end

num=[];

return;

%EOF