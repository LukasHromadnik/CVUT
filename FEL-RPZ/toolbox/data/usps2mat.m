% USPS2MAT converts USPS database to Matlab data file (MAT).
%
% Description: 
%  It creates MAT-file containing training and testing part
%  of the USPS database of handwritten numerals.
%
% Input: 
%  It requires input text files "zip.test" and "zip.train"
%  which can be downloaded from:
%
%  http://www-stat-class.stanford.edu/~tibs/ElemStatLearn/datasets/zip.train.gz
%  http://www-stat-class.stanford.edu/~tibs/ElemStatLearn/datasets/zip.test.gz
%  
% Output:
%  The output MAT-file "usps.mat" contains: 
%   trn.X [256 x 7291] Images of training numerals stored as columns.
%   trn.y [1 x 7291] Labels y==1 -->"1", ..., y==9-->"9",... y==10-->"0".
%
%   tst.X [256 x 2007] Images of testing numerals stored as columns.
%   tst.y [1 x 2007] Labels.
%
% Example:
%  The following code displays the first training image.
%   load usps.mat;
%   img = reshape(trn.X(:,1),16,16);
%   figure; 
%   imshow(img,[]);
% 

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 07-jun-2004, VF
% 10-mar-2004, VF

trn_data = load('zip.train','-ascii');
tst_data = load('zip.test','-ascii');

% processing training data
trn.y = trn_data(:,1);
trn.y(find(trn.y==0)) = 10;

tmp = trn_data(:,2:end)';
trn.X = zeros(256, size(tmp,2));
fprintf('Processing training data');
for i=1:size(tmp,2),
  fprintf('.');
  dummy = reshape( tmp(:,i), 16, 16 )';
  trn.X(:,i) = (dummy(:)+1)/2;
end
fprintf('\n');
  
% processing testing data
tst.y = tst_data(:,1);
tst.y(find(tst.y==0)) = 10;

tmp = tst_data(:,2:end)';
tst.X = zeros(256, size(tmp,2));
fprintf('Processing testing data');
for i=1:size(tmp,2),
  fprintf('.');
  dummy = reshape( tmp(:,i), 16, 16 )';
  tst.X(:,i) = (dummy(:)+1)/2;
end
fprintf('\n');

% 
name = 'USPS database of handwritten numerals';
save usps trn tst name;

% EOF
