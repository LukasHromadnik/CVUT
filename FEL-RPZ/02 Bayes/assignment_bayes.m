load data_33rpz_cv02

%%  discrete

W = [0 1;
     1 0];
% W = [0 5; 1 0];

q_discrete = find_strategy_discrete(discreteA, discreteC, W)

visualize_discrete(discreteA, discreteC, q_discrete)

R_discrete = bayes_risk_discrete(discreteA, discreteC, W, q_discrete)

% result not used, inspect the values e.g. with hist function
measurements_discrete = compute_measurement_lr_discrete(images_test);

% result not used, inspect the values 
labels_estimated_discrete = classify_discrete(images_test, q_discrete);

error_discrete = classification_error_discrete(images_test, labels_test, q_discrete)

%% visualization
I_A = images_test(:,:,labels_estimated_discrete == 1);
I_C = images_test(:,:,labels_estimated_discrete == 2);
figure, subplot(1,2,1), montage(permute(I_A, [1 2 4 3])), title A;
subplot(1,2,2), montage(permute(I_C, [1 2 4 3])),title C;

%% continous

% we are searching for bayesian strategy for 2 normal distributions and
% zero-one cost function
% W = [0 1;
%      1 0];

% initialize stpr toolbox 
%run('../../../solutions/3rdparty/stprtool/stprpath.m')

% result not used, inspect the values e.g. with hist function
measurements_cont = compute_measurement_lr_cont(images_test);

q_cont = find_strategy_2normal(contA, contC)
%visualize_2norm(contA, contC, q_cont)

R_cont = bayes_risk_2normal(contA, contC, q_cont)

% result not used, inspect the values 
labels_estimated_cont = classify_2normal(images_test, q_cont);

error_cont = classification_error_2normal(images_test, labels_test, q_cont)

%% visualization
I_A = images_test(:,:,labels_estimated_cont == 1);
I_C = images_test(:,:,labels_estimated_cont == 2);
figure, subplot(1,2,1), montage(permute(I_A, [1 2 4 3])), title A;
subplot(1,2,2), montage(permute(I_C, [1 2 4 3])),title C;