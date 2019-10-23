% Assigment: PARZEN WINDOW ESTIMATION

%% init
load data_33rpz_parzen.mat
%run('../../../../solutions/3rdparty/stprtool/stprpath.m')
run('../toolbox/stprpath.m')

%% Tasks, part 1
%% measurements
x = compute_measurement_lr_cont(trn.images);

% splitting the trainning data into classes
xA = x(trn.labels == 1);
xC = x(trn.labels == 2);

%% computing the histograms of training data
[bins_A, centers_A] = hist(xA, 20);
bins_A = bins_A / (sum(bins_A)*(centers_A(2)-centers_A(1)));

[bins_C, centers_C] = hist(xC);
bins_C = bins_C / (sum(bins_C)*(centers_C(2)-centers_C(1)));

%% estimating conditional probability using Parzen window
x_range = min(xA):100:max(xA);
h = [100 500 1000 2000];
p = my_parzen(x_range, xA, h(1));
%% plots of the estimates
figure;
% h = 100
subplot(2,2,1);
bar(centers_A, bins_A);
hold on;
plot(x_range, p, 'r', 'linewidth', 2);
title('h = 100');
xlabel('x');
ylabel('p(x|A)');
ylim([0 4.5e-4]);
grid on;

% h = 300
subplot(2,2,2);
bar(centers_A, bins_A);
hold on;
p = my_parzen(x_range, xA, h(2));
plot(x_range, p, 'r', 'linewidth', 2);
title('h = 500');
xlabel('x');
ylabel('p(x|A)');
ylim([0 4.5e-4]);
grid on;
% h = 1000
subplot(2,2,3);
bar(centers_A, bins_A);
hold on;
p = my_parzen(x_range, xA, h(3));
plot(x_range, p, 'r', 'linewidth', 2);
title('h = 1000');
xlabel('x');
ylabel('p(x|A)');
ylim([0 4.5e-4]);
grid on;
% h = 3000
subplot(2,2,4);
bar(centers_A, bins_A);
hold on;
p = my_parzen(x_range, xA, h(4));
plot(x_range, p, 'r', 'linewidth', 2);
title('h = 2000');
xlabel('x');
ylabel('p(x|A)');
ylim([0 4.5e-4]);
grid on;

%% Tasks, part 2
%% 10-fold cross-validation init
h_range = 100:50:1000;
num_folds = 10;

%% class A cross-validation
rand('seed', 42);   % needed only for upload system, to test the correctness of the code

[itrn, itst] = crossval(length(xA), 10);
fun_Lh = @(h)compute_Lh(itrn, itst, xA, h);
Lh = arrayfun(fun_Lh, h_range);

%% optimal value of parameter h
h_bestA = fminbnd(@(h)-compute_Lh(itrn, itst, xA, h), h_range(1), h_range(end));
Lh_bestA = fun_Lh(h_bestA);

%% plots of optimal h
figure;
subplot(1,2,1);
plot(h_range, Lh);
hold on;
plot(h_bestA, Lh_bestA,'or');
cur_ylim = get(gca, 'ylim');
plot([h_bestA h_bestA], [cur_ylim(1) Lh_bestA], '--r');
title('10-fold cross-validation');
xlabel('h');
ylabel('L(h)');
grid on;

p = my_parzen(x_range, xA, h_bestA);
subplot(1,2,2);
bar(centers_A, bins_A);
hold on;
plot(x_range, p, 'r', 'linewidth', 2);
grid on;
title('Best bandwidth h for class A');
xlabel('x');
ylabel('p(x|A)');

%% class C cross-validation
rand('seed', 42);   % needed only for upload system, to test the correctness of the code

[itrn, itst] = crossval(length(xC), 10);
fun_Lh = @(h)compute_Lh(itrn, itst, xC, h);
Lh = arrayfun(fun_Lh, h_range);

%% optimal value of parameter h
h_bestC = fminbnd(@(h)-compute_Lh(itrn, itst, xC, h), h_range(1), h_range(end));
Lh_bestC = fun_Lh(h_bestC);

%% plots of optimal h
figure;
subplot(1,2,1);
plot(h_range, Lh);
hold on;
plot(h_bestC, Lh_bestC,'or');
title('10-fold cross-validation');
xlabel('h');
ylabel('L(h)');
grid on;

p = my_parzen(x_range, xC, h_bestC);
subplot(1,2,2);
bar(centers_C, bins_C);
hold on;
plot(x_range, p, 'r', 'linewidth', 2);
grid on;
title('Best bandwidth h for class C');
xlabel('x');
ylabel('p(x|C)');

%% Bayesian classifier
x_test = compute_measurement_lr_cont(tst.images);

% computing a priori probabilities
pA = estimate_prior(1, trn.labels);
pC = estimate_prior(2, trn.labels);

labels = classify_bayes_parzen(x_test, xA, xC, pA, pC, h_bestA, h_bestC)

% showing images
show_classification(tst.images, labels, 'AC'); 

% classification error
bayes_error = sum(abs(tst.labels - labels)) / length(labels);
%}