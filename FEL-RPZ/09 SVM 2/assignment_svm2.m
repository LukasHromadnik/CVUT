%% Asignment: Support Vector Machines 2

clc;
close all;
clearvars;

%% Init STPR Toolbox

% Change this:
run('../toolbox/stprpath.m');

%{
%% %% Tasks, part 1 - Kernel SVM

%% %% Load data

load('flower.mat');
% load('tmp/data_33rpz_svm_toy.mat');

y(y == 2) = -1;

C = 10;
options = c2s({'verb', 1, 'tmax', inf, 'kernel', 'rbf', 'sigma', 0.1});

model = my_kernel_svm(X, y, C, options);

% visualization 
y(y == -1) = 2;
figure(1);
ppatterns(X, y);
pboundary(model);
title('RBF kernel, \sigma = 0.1');

saveas(gcf, 'flower_rbf.png');
%}
%% %% Tasks, part 2 - Model selection on OCR data
%{
% Train
clc;
clear all;

load('ocr_data_2D_trn.mat');

Cs = [0.0001 0.001 0.01 0.1];
ds = [5, 10, 15];

rand('seed', 42);
[itrn, itst] = crossval(size(X, 2), 4);
TstErrs = [];

for C = Cs
    for d = ds
        options = c2s({'verb', 0, 'tmax', inf, 'kernel', 'polynomial', 'd', d});
        TstErrs(end + 1) = compute_kernel_TstErr(itrn, itst, X, y, C, options);
    end
end

[minTstErr, idx] = min(TstErrs);

cs_idx = ceil(idx / size(ds, 2));
ds_idx = mod(idx, size(ds, 2));

C_opt = Cs(cs_idx);
d_opt = ds(ds_idx);

% Train with optimal C and d

y(y == 2) = -1;
options = c2s({'verb', 0, 'tmax', inf, 'kernel', 'polynomial', 'd', d_opt});
model = my_kernel_svm(X, y, C_opt, options);
classif = classif_kernel_svm(X, model);
trn_error = abs(sum(classif - y));

% visualization 
y(y == -1) = 2;
figure;
ppatterns(X, y);
pboundary(model);
title(['Polynomial kernel, d = ' num2str(d_opt) ' TRN data']);

saveas(gcf, 'ocr_polynomial_kernel_trn.png');

% Test

load('ocr_data_2D_tst.mat');

y(y == 2) = -1;

classif = classif_kernel_svm(X, model);
error = abs(sum(classif - y)) / (2 * size(classif, 2))

% visualization 
y(y == -1) = 2;
figure;
ppatterns(X, y);
pboundary(model);
title(['Polynomial kernel, d = ' num2str(d_opt) ' TST data']);

saveas(gcf, 'ocr_polynomial_kernel_tst.png');

classif(classif==-1) = 2;
show_classification(tst.images, classif, 'AC');
saveas(gcf, 'ocr_polynomial_kernel_classif.png');
%}

%% %% Tasks, part 3 - Digit classification

% Train kernel SVM

clc;
clear all;

load('imdb.mat');
X;
for i = 1:400
    imgs = imdb.images.data(:,:,:,i);
    X(end + 1, :) = reshape(imgs, numel(imgs), 1);
end

y = imdb.images.labels(1:400);

Cs = [0.01 0.1 1 10];
sigmas = [0.5, 0.9, 1.5];

rand('seed', 42);
[itrn, itst] = crossval(size(X, 2), 5);
TstErrs = [];

for C = Cs
    for sigma = sigmas
        options = c2s({'verb', 0, 'tmax', inf, 'kernel', 'rbf', 'sigma', sigma});
        TstErrs(end+1) = compute_kernel_TstErr(itrn, itst, X, y, C, options);
    end
end

[minTstErr, idx] = min(TstErrs);

cs_idx = ceil(idx / size(sigmas, 2));
sigmas_idx = mod(idx, size(sigmas, 2));

C_opt = Cs(cs_idx)
sigma_opt = sigmas(sigmas_idx);

options = c2s({'verb', 0, 'tmax', inf, 'kernel', 'rbf', 'sigma', sigma_opt});
model = my_kernel_svm(X, y, C_opt, options);

%trn_error = ...

% Evaluate on test data 

X;
for i = 500:700
    imgs = imdb.images.data(:,:,:,i);
    X(end + 1, :) = reshape(imgs, numel(imgs), 1);
end

yclass = imdb.images.labels(500:700);

show_mnist_classification(X, yclass);

saveas(gcf, 'mnist_tst_classif.png');

classif = classif_kernel_svm(X, model);
error = abs(sum(classif - y)) / (2 * size(classif, 2))
