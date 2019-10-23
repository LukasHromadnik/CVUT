% define training examples and their labels
X = [ [0; 1] [1; 1] [0; 0] [1; 0] ];
Y = [ +1     +1     -1     -1];
m = length(Y);

% kernel matrix for linear kernel
K = (X' * X);

% run libsvm with user-defined kernel
C   = 1;
Svm = svmtrain(Y(:), [[1:m]' K], ['-s 0 -t 4 -c ' num2str(C)]);

% get parameters of kernel classifier
bias           = -Svm.rho;
alpha          = zeros( m, 1);
alpha(Svm.SVs) = Svm.sv_coef(:);

% predict training examples
score = K * alpha + bias;
predY = 2 * double(score >= 0) - 1;

% compute training error
trnErr = mean(predY(:) ~= Y(:));
