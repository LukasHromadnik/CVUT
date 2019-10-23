% Load input data
load('dns/dns_data_kernel');

X = Trn.X;
Y = Trn.Y;
m = length(Y);
K = [[1:m]' Trn.K];
C = [0.01 0.1 1 10 100 1000];

val_m = length(Val.Y);
val_K = [[1:val_m]' Val.K];

training_error = [];
validation_error = [];

for c = C
    Svm = svmtrain(Y(:), K, ['-t 4 -c ' num2str(c)]);
    [~, err, ~] = svmpredict(Trn.Y, K, Svm);
    training_error = [training_error, 100 - err(1:1)];
    [~, err, ~] = svmpredict(Val.Y, val_K, Svm);
    validation_error = [validation_error, 100 - err(1:1)];
end

% Plot training error vs validation error
plot(C, training_error, C, validation_error);
set(gca, 'xscale', 'log');
legend('Training error', 'Validation error');
saveas(gcf, 'error.png')

% Minimal epsilon
gamma = 0.99;
sqrt((log(2) - log(1 - gamma)) / (2 * m))
