% This script tests functionality of the XTAL interface.
%
% The script runs regression methods implemented in the XTAL 
% package on a test problem and visualizes obtained results.
%

% load training/testing data
%-----------------------------------------

tmp =load('train.txt','-ascii');
trn_data.X = tmp(:,1)';
trn_data.y = tmp(:,2);

tmp =load('test.txt','-ascii');
tst_data.X = tmp(:,1)';
tst_data.y = tmp(:,2);

% experiment setup
%-----------------------------------------

exp(1).method = 'ANN1';
exp(1).params = [ 5; 10; 20; 40; 100];

exp(2).method = 'MRS1';
exp(2).params = [[100,0]; [100,2]; [100,5]; [100,9]; [200,0]; [200,2]; [200,5]; [200,9]];

exp(3).method = 'PPR1';
exp(3).params = [1; 2; 5; 8; 10; 20; 50];

exp(4).method = 'CTM1';
exp(4).params = [[2,0]; [2,2]; [2,5]; [2,9]; [3,0]; [3,2]; [3,5]; [3,9]];

exp(5).method = 'KNN1';
exp(5).params = [2; 3; 4; 5; 6; 7; 8; 9; 10; 11];

%-----------------------------------------

for i=1:length(exp)
    
    fprintf('\nExperiment: %s\n', exp(i).method);
    fprintf('---------------------------------------\n');
    
    [ypred,exp(i).nrms,exp(i).rms0,exp(i).nmin] = xtal(trn_data,tst_data,exp(i).method,exp(i).params);

    [exp(i).min_nrms,exp(i).best_param_idx] = min(exp(i).nrms);
    
    fprintf('Minimal NRMS=%f achieved with parameter ( ', exp(i).min_nrms);
    fprintf('%d ', exp(i).params(exp(i).best_param_idx,:));
    fprintf(')\n');

    h0=figure;
    set(h0,'Name',exp(i).method);
    h1=plot(trn_data.X,trn_data.y,'kx');
    hold on;
    h2=plot(tst_data.X,tst_data.y,'rx');
    h3=plot(tst_data.X,ypred(exp(i).best_param_idx,:),'bx--');
    legend([h1 h2 h3],'traing examples','testing examples','prediction');
end


[min_nrms,best_method] = min([exp.min_nrms]);
fprintf('\n** Summary **\n\n');
fprintf('Method         NRMS         NMIN  parameter\n');
fprintf('--------------------------------------------------\n');
for i=1:length(exp)
    fprintf('%s   %12.6f %12.6f  ( ',...
            exp(i).method,exp(i).min_nrms,exp(i).nmin(exp(i).best_param_idx));
    fprintf('%d ', exp(i).params(exp(i).best_param_idx,:));
    fprintf(')');

    if i == best_method
        fprintf('         best result');
    end
    fprintf('\n');
end
