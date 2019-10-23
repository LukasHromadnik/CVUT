function [ypred,nrms,rms0,nmax]= xtal(trn_data,tst_data,method,params,verb)
% XTAL is interface to the XTAL regression package. 
% 
% Synopsis:
%  [ypred,nrms,rms0,nmax] = xtal(trn_data,tst_data,method,params)
%
% Description:
%  This function provides Matlab interface to the XTAL regression package. 
%
%  The XTAL package is accessed from Matlab through the program "xtal". 
%  Data are exchanged using text files created in the XTAL root directory 
%  whenever this function is issued. 
%  
%  Requirements:
%   1. XTAL package is installed. XTAL can be downloaded from
%      http://www.ece.umn.edu/users/cherkass/ee8591/software/xtal.html
%      
%      The STPRtool comes with binaries compiled for Linux and Windows 
%      (to run XTAL under Windows it is required that CYGWIN from www.cygwin.com/ 
%      is installed).
%
%   2. XTAL root directory and the command line to run XTAL are specified 
%      in "xtal_rootdir.m". See "help xtal_rootdir".
%
%      For Linux and Windows the XTAL root directory and the command line are 
%      generated automatically. Hence the XTAL interface should work out of the box.  
%      If not you have to do both above mentioned steps manually.
% 
% Inputs:
%  trn_data.X [nDim x nTrnData] training inputs.
%  trn_data.y [nTrnData x 1] training outputs.
%
%  tst_data.X [nDim x nTstData] testing inputs.
%  tst_data.y [nTstData x 1] testing outputs.
%
%  method [string] Regression method supported by XTAL; it must be 
%    one of the following strings: 'ANN1','PPR1','CTM1','KNN1','MRS1' 
%  params [nRuns x nParams] Parameters of the selected method. 
%    See XTAL documentation for more info.
%  verb [1x1] if 1 (default) then progress info is displayed. 
%
% Outputs:
%  ypred [nRuns x nTstData] Predicted test outputs.
%  nrms [nRuns x 1] Normalized RMS error for each run.
%  rms0 [nRuns x 1] RMS error of the 0-th order model.
%  nmax [nRuns x 1] Normalized maximal deviation.
%
% Example:
%  Run test script
%    xtal_test
%

    
[XTAL_RootDir,XTAL_CmdLine] = xtal_rootdir;

%----------------------------------------------------
    
Methods = {'ANN1','PPR1','CTM1','KNN1','MRS1'};
ResultFiles = {'ann1.res','ppr.res','fit1','knn.res','mars.res'};

if nargin < 4
    error('XTAL function expetct four input arguments at least.');
end
if nargin < 5
    verb = 1;
end
if isempty(strmatch(method,Methods,'exact'))
    error('Input argument "method" does not match any supported method.');
end


nTstData = length(tst_data.y);
nTrnData = length(trn_data.y);
nRuns = size(params,1);


% save training/testing data to XTAL RootDir
%---------------------------------------------
data = [trn_data.X' trn_data.y(:)];
save([XTAL_RootDir 'train.txt'],'data','-ASCII');
trn_fname = 'train.txt';

data = [tst_data.X' tst_data.y(:)];
save([XTAL_RootDir 'test.txt'],'data','-ASCII');
tst_fname = 'test.txt';

% create filename.con 
%---------------------------------------------
fname = [XTAL_RootDir 'filename.con'];
fid = fopen(fname,'w');
if fid == -1 
    error(sprintf('Cannot open: %s .',fname));
end
fprintf(fid,[trn_fname ' ' tst_fname]);
fclose(fid);

ypred = zeros(nRuns,nTstData);
rms0 = zeros(nRuns,1);
nrms = zeros(nRuns,1);
nmax = zeros(nRuns,1);

max_tst_y = max(tst_data.y);
min_tst_y = min(tst_data.y);

method_idx = strmatch(method,Methods,'exact');
if isempty(method_idx)
   error('Improper value for the argument "method"');
end

% call xtal-method for all parameters
%---------------------------------------------
for i=1:size(params,1)
   
    % create operate.con file
    fname = [XTAL_RootDir 'operate.con'];
    fid = fopen(fname,'w');
    if fid == -1 
        error(sprintf('Cannot open: %s .',fname));
    end
    fprintf(fid,'%s ',method);
    for j=1:size(params,2)-1
        fprintf(fid,'%d,', params(i,j));
    end
    fprintf(fid,'%d\n', params(i,end));
    fclose(fid);

    if verb
        fprintf('Running %s with parameter ( ', method);
        fprintf('%d ', params(i,:));
        fprintf(') ... ');
    end

    res_fname = [XTAL_RootDir ResultFiles{method_idx}];
    if exist(res_fname)
        delete(res_fname);
    end

    % run xtal
    [a,b] = system(XTAL_CmdLine); 

    if verb
        fprintf('done.');
    end
        
    % load predicted outputs
    data = load(res_fname,'-ASCII');
    ypred(i,:) = data(:,end)';

    % compute RMS of 0-th order method
    rms0(i) = sum((tst_data.y - mean(trn_data.y)).^2);
    rms0(i) = sqrt(rms0(i)/(nTstData+1));

    % compute normalized RMS of the prediced outputs
    nrms(i) = sum((ypred(i,:)-tst_data.y(:)').^2);
    nrms(i) = sqrt(nrms(i)/length(tst_data.y))/rms0(i);

    % compute normalized maximal deviation
    nmax(i) = max(ypred(i,:)-tst_data.y(:)')/(max_tst_y-min_tst_y);
    
    if verb
        fprintf('  NRMS: %f  NMAX: %f\n', nrms(i), nmax(i));
    end
    
end

return;
