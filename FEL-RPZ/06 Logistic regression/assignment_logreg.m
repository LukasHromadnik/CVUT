% RPZ assigment: Logistic regression


%% Init
run('../toolbox/stprpath.m');

%% Classification of letters A and C
%--------------------------------------------------------------------------
% Load training data and compute features
load ('data_33rpz_logreg.mat');

% Prepare the training data
trainX = prepare_data(trn.images);

% Training - gradient descent of the logistic loss function
% Start at a fixed point:
w_init = [-7; -8];
% or start at a random point:
%w_init = 20 * (rand(size(trainX, 1), 1) - 0.5);
epsilon = 1e-2;
[w, wt, Et] = logistic_loss_gradient_descent(trainX, trn.labels, w_init, epsilon);

% Plot the progress of the gradient descent
%plot(Et);
%saveas(gcf, 'E_progress_AC.png');
%clf(gcf);

%
%plot_gradient_descent(trainX, trn.labels, @logistic_loss, w, wt, Et);
%saveas(gcf, 'w_progress_2d_AC.png');
%clg(gcf);

% plot aposteriori probabilities

thr = get_threshold(w);
posterior = @(e) 1 / (1 + exp(-e));

plot(thr, [0 1]);




% Prepare the test data
%testX = prepare_data(tst.images);
    
% Classify letter test data and calculate classification error
%classifiedLabels = classify_images(testX, w);

%testError = ???;
%fprintf('Letter classification error: %.2f%%\n', testError * 100);

% Visualize classification results
%clf(gcf);
%show_classification(tst.images, classifiedLabels, 'AC');
%saveas(gcf, 'classif_AC.png');



%% Classification of MNIST digits
%--------------------------------------------------------------------------
%{
% Load training data
load('mnist_01_trn');

% prepare the training data
Xtrain = [ones(size(X, 2), 1) X']';
%y = y';

% Training - gradient descent of the logistic loss function
w_init = rand(size(Xtrain, 1), 1);
epsilon = 1e-2;
[w, ~, Et] = logistic_loss_gradient_descent(Xtrain, y, w_init, epsilon);

% Plot the progress of the gradient descent
% plotEt
clf(gcf);
plot(Et);
saveas(gcf, 'E_progress_MNIST.png');
    
% Load test data
load('mnist_01_tst');
Xtest = [ones(size(X, 2), 1) X']';

% prepare the training data

% Classify MNIST test data and calculate classification error
classifiedLabels = classify_images(Xtest, w);
%testError = ???;
%fprintf('MNIST digit classification error: %.2f%%\n', testError * 100);

% Visualize classification results
clf(gcf);
show_mnist_classification(Xtest(2:end, :), classifiedLabels);
saveas(gcf, 'classif_MNIST.png');
%}