function [sel_inx,Alpha,Z,kercnt,MsErr,MaxErr]=...
  greedyappx(X,ker,arg,m,p,mserr,maxerr,verb) 
% GREEDYAPPX Kernel greedy data approximation.
%
% Synopsis:
%  [Sel_inx,Alpha,Z,Kercnt,MsErrors,MaxErrors] = ...
%     greedyappx(X,Ker,Arg,M,P,MsErr,Maxerr,Verb) 
%
% Description:
%  The input column vectors are assumed to be represented
%  in a kernel feature space given by (ker,arg) (see help kernel).
%  This function aims to select a subset X(:,Sel_inx) such
%  that linear span of X(:,Sel_inx) in the feature space 
%  approximates well the linear span of X in the feature space.
%  The vectors are selected such that the mean square reconstruction
%  error in the feature space (the same objective as has Kernel PCA) 
%  is minimized by greedy algorithm. The algorithm selects vectors
%  until on of the following stopping  conditions is achieved:
%    - number of vectors achieves m 
%    - maximal reconstruction error drops below maxerr 
%    - mean squared sum of reconstruction errors less than mserr. 
%  
%  The images of X(:,Inx_sel) in the features form a basis.
%  The projection of input vector x into the basis is done by
%     z = Alpha*kernel(x,X(:,Sel_inx),Ker,Arg)
%
%  For more info refer to V.Franc: Optimization Algorithms for Kernel 
%  Methods. Research report. CTU-CMP-2005-22. CTU FEL Prague. 2005.
%  ftp://cmp.felk.cvut.cz/pub/cmp/articles/franc/Franc-PhD.pdf .
%
% Input:
%  X [Dim x Num_data] Input data.
%  Ker [string] Kernel identifier. See help of KERNEL for more info.
%  Arg [...] Argument of selected kernel.
%  M [1x1] Maximal number of vector used for approximation.
%  P [1x1] Depth of search for each basis vector.
%  MsErr [1x1] Desired mean sum of squared reconstruction errors.
%  MaxErr [1x1] Desired maximal reconstruction error.
%  Verb [1x1] If 1 then infor about process is displayed.
%
% Output:
%  Sel_inx [1 x M] Indices of selected vector, i.e., S = X(:,Sel_inx).
%  Alpha [M x M] Defines projection into the found basis (see above).
%  Z [M x Num_data] Training data projected into the found basis.
%  Kercnt [1 x 1] Number of used kernel evaluations.
%  MsErrors [1 x M] Mean square reconstruction error wrt to selected 
%   basis vectors. MsErr(end) is the resulting error.
%  MaxErrors [1 x M] Maximal squared reconstruction error (of the worst
%   input example) wrt. selcetd basis vectors.
%
% Example:
%  type greedykpca
%
% See also 
%  GREEDYKPCA.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 09-sep-2005, VF
% 12-feb-2005, VF, New help made
% 10-dec-2004, VF, tmp(find(Errors<=0)) = -inf; added to evoid num errors.
% 5-may-2004, VF
% 13-mar-2004, VF
% 10-mar-2004, VF
% 9-mar-2004, addopted from greedyappx

if nargin < 5, mserr=1e-6; end
if nargin < 6, maxerr=1e-6; end
if nargin < 7, verb=0; end

[dim,num_data]=size(X);

if verb, 
  fprintf('Greedy data approximation.\n');
  fprintf('Setting: ker=%s, arg=%f, m=%d, eps=%f\n', ker,arg,m,maxerr); 
end

kercnt=0;
Errors = diagker(X,ker,arg)';  kercnt = kercnt+num_data;
Z = zeros(m,num_data);

MsErr = [];
MaxErr = [];
Alpha=zeros(m,m);
SV = zeros(dim,m);
sel_inx=[];
work_inx = [1:num_data];

for i=1:m,

  if i == 1,
    [tmp_sel_inx,tmp_Alpha,tmp_Z,tmp_kercnt,tmp_MsErr,tmp_MaxErr]=...
       upperbound_greedyappx(X,ker,arg,p,0,1e-12,verb);
    kercnt = kercnt+tmp_kercnt;
  else
    init_model.Alpha = Alpha(1:i-1,1:i-1);
    init_model.Z = Z(1:i-1,:);
    init_model.Errors = Errors;

    [tmp_sel_inx,tmp_Alpha,tmp_Z,tmp_kercnt,tmp_MsErr,tmp_MaxErr]=...
       upperbound_greedyappx(X,ker,arg,p,0,1e-12,verb,init_model);
    kercnt = kercnt+tmp_kercnt;
  end

  tmp_Z = tmp_Z(i:size(tmp_Z,1),:);
  A = tmp_Z*tmp_Z';

  tmp1 = sum(tmp_Z.^2,1);
  tmp1(find(tmp1==0))=inf;
  tmp = sum((A*tmp_Z).*tmp_Z,1)./tmp1;
  tmp(sel_inx) = -inf; tmp(find(Errors<=0)) = -inf;
  [dummy,new_inx]=max(tmp);
 
  % orthonormalization
  sel_inx = [sel_inx new_inx];
  tmp = kernel( X(:,new_inx), X(:,work_inx), ker, arg );
  kercnt=kercnt+num_data-i;

  if i > 1,
     Z(i,work_inx) = ...
       (tmp - Z(1:i-1,new_inx)'*Z(1:i-1,work_inx))/sqrt(Errors(new_inx));

     Alpha(i,:) = - Z(1:i-1,new_inx)'*Alpha(1:i-1,:);
     Alpha(i,i) = 1;
     Alpha(i,:) = Alpha(i,:)/sqrt(Errors(new_inx));
  else
     Z(1,:) = tmp/sqrt(Errors(new_inx));
     Alpha(1,1) = 1/sqrt(Errors(new_inx));
  end

  % Error(i) = k(i,i)-k'(i,i)
  Errors(work_inx) = Errors(work_inx) - Z(i,work_inx).^2;
  Errors(find(Errors<0)) = 0;
  work_inx(find(work_inx==new_inx)) = [];
  
  % store errors
  MsErr = [MsErr, sum(Errors)/num_data];
  MaxErr = [MaxErr, max(Errors)];
  
  if verb,
    fprintf('%d: maxerr=%f, mserr=%f, inx=%d\n', ...
        i,MaxErr(end), MsErr(end), new_inx);
  end
  
  % evaluate stopping conditions:
  if maxerr >= MaxErr(end) | mserr >= MsErr(end),
    break;
  end
end


% cut off non-used memory if number of used base vector is less than m
Alpha=Alpha(1:i,1:i);
Z = Z(1:i,:);
return;

%=================================================
function [sel_inx,Alpha,Z,kercnt,MsErr,MaxErr]=...
  upperbound_greedyappx(X,ker,arg,m,mserr,maxerr,verb,init_model) 

[dim,num_data]=size(X);

kercnt=0;
sel_inx=[];              % indices of seleted base vectors
work_inx = [1:num_data]; % indices of the rest 
MsErr = [];
MaxErr = [];

if nargin < 8,
  Errors = diagker(X,ker,arg)';
  Z = zeros(m,num_data);
  Alpha=zeros(m,m);
  curr_m = 0;
else
  Errors = init_model.Errors;
  curr_m = size(init_model.Z,1);
  m = m + curr_m;
  Z = zeros(m,num_data);
  Alpha=zeros(m,m);

  Z(1:curr_m,:) = init_model.Z;
  Alpha(1:curr_m,1:curr_m) = init_model.Alpha;
end

if verb, fprintf('('); end

for i=curr_m+1:m,
  
  % find vector with highest reconstruction error
  [curr_maxerr,new_inx] = max( Errors );
  sel_inx = [sel_inx,new_inx];

  % orthonormalization
  tmp = kernel( X(:,new_inx), X(:,work_inx), ker, arg ); 
  kercnt = kercnt + num_data - i;

  if i > 1,
     Z(i,work_inx) = ...
       (tmp - Z(1:i-1,new_inx)'*Z(1:i-1,work_inx))/sqrt(Errors(new_inx)); 

     Alpha(i,:) = - Z(1:i-1,new_inx)'*Alpha(1:i-1,:);
     Alpha(i,i) = 1;
     Alpha(i,:) = Alpha(i,:)/sqrt(Errors(new_inx));
  else
     Z(1,:) = tmp/sqrt(Errors(new_inx));
     Alpha(1,1) = 1/sqrt(Errors(new_inx));
  end

  % Error(i) = k(i,i)-k'(i,i)
  Errors(work_inx) = Errors(work_inx) - Z(i,work_inx).^2;  
%  Errors(sel_inx)=zeros(1,length(sel_inx));
  work_inx(find(work_inx==new_inx)) = [];
    
  % store errors
  MsErr = [MsErr, sum(Errors)/num_data];
  MaxErr = [MaxErr, max(Errors)];
  
  if verb,
    fprintf('.', i, MsErr(end) );
  end
  
  % evaluate stopping conditions:
  if maxerr >= MaxErr(end) | mserr >= MsErr(end),
    break;
  end
end

if verb, fprintf(')\n'); end

% cut off non-used memory if number of used base vector is less than m
Alpha=Alpha(1:i,1:i);
Z = Z(1:i,:);

return;
% EOF