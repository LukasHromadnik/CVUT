load('data_33rpz_logreg.mat');

trainX = prepare_data(trn.images);
w_init = [-7; -8];
epsilon = 1e-2;
[w, wt, Et] = logistic_loss_gradient_descent(trainX, trn.labels, w_init, epsilon);

thr = get_threshold(w);

values = sum(w .* trainX);
posterior = @(e) 1 / (1 + exp(-e));
p1 = arrayfun(posterior, values);
p2 = arrayfun(posterior, -values);

hold on;
scatter(compute_measurements(trn.images), p1);
scatter(compute_measurements(trn.images), p2);
plot([thr thr], [0 1]);
hold off;

saveas(gcf, 'aposteriori.png');