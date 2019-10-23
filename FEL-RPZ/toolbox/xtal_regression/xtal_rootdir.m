function [RootDir,CmdLine] = xtal_rootdir()
% XTAL_ROOTDIR returns root directory and command line to issue XTAL.
% 
% Synopsis:
%  [RootDir,CmdLine] = xtal_rootdir
%
% Description:
%  This function returns the root directory of XTAL package and 
%  the command line which, if executed, changes directory to RootDir
%  and issues XTAL.
% 
%  For Linux and Windows, RootDir and CmdLine are generated 
%  automatically. 
%
%  In the case that it does not work automatically, you have to set 
%  RootDir and CmdLine manually at the beggining of this file. 
%
%  For example in Linux
%    Rootdir = '/home/username/stprtool/xtal_regression/xtal_linux/';
%    CmdLine = 'cd /home/username/stprtool/xtal_regression/xtal_linux/; ./xtal';
%
%  and in Windows
%
%    Rootdir = 'c:/stprtool/xtal_regresssion/xtal_win32/';
%    CmdLine = 'cd c:/stprtool/xtal_regresssion/xtal_win32/ & xtal.exe';
%  
% 
        
%RootDir = 'D:\work-mike\xtal\xtal_linux\';
%RootDir='D:\XTAL\xtal_win32\xtal_linux\';

RootDir = [];
CmdLine = [];

%-------------------------------------------------------------
% Try automatic locatiozation of the root directory if not given
%-------------------------------------------------------------
if isempty(RootDir)
   
   a = which('xtal');
   b = findstr(a,'xtal.m');
   c = a(1:b-1);
    
   switch computer
     case 'GLNX86'
       RootDir = [c 'xtal_linux/'];
       
     case 'PCWIN'
       RootDir = [c 'xtal_win32\'];
       
     otherwise
       error('Supply RootDir and CmdLine to "xtal_rootdir.m".');
   end    
   
   if ~exist(RootDir)
       error('Supply RootDir and CmdLine to "xtal_rootdir.m".');
   end   
end

if isempty(CmdLine)
   switch computer
     case 'GLNX86'
       CmdLine = ['cd ' RootDir ' ; ./xtal'];
       
     case 'PCWIN'
       CmdLine = ['cd ' RootDir ' & xtal.exe'];
     
     otherwise
       error('Supply RootDir and CmdLine to "xtal_rootdir.m".');
   end    
       
end

return;