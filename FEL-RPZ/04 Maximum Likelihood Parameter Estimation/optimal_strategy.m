function ClassError = optimal_strategy(trn, tst)

x = compute_measurement_lr_cont(trn.images);

DA.Prior= estimate_prior(1, trn.labels);
DC.Prior = estimate_prior(2, trn.labels);

x_A = x(trn.labels == 1);
x_C = x(trn.labels == 2);

[DA.Mean, DA.Sigma] = mle_normal(x_A);
[DC.Mean, DC.Sigma] = mle_normal(x_C);

% Classify images
strategy = find_strategy_2normal(DA, DC);
q_x = classify_2normal(tst.images, strategy);

ClassError = mean(abs(q_x - tst.labels));

% Classification error
%ClassError = classification_error_2normal(tst.images, tst.labels, strategy);

% Displaying images

show_classification(tst.images, q_x, 'AC');
end