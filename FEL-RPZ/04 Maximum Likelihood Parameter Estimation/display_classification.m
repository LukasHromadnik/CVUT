function display_classification(tst)
x = compute_measurement_lr_cont(tst.images);

DA.Prior = estimate_prior(1, tst.labels);
DC.Prior = estimate_prior(2, tst.labels);

x_A = x(tst.labels == 1);
x_C = x(tst.labels == 2);

[DA.Mean, DA.Sigma] = mle_normal(x_A);
[DC.Mean, DC.Sigma] = mle_normal(x_C);

% Classify images
strategy = minmax_strategy_cont(DA, DC);
q_x1 = classify_2normal(tst.images, strategy);
ClassError = classification_error_2normal(tst.images, tst.labels, strategy);

strategy = find_strategy_2normal(DA, DC);
q_x = classify_2normal(tst.images, strategy);

if isequal(q_x1, q_x)
    ano = 1
end

% Classification error
ClassError = classification_error_2normal(tst.images, tst.labels, strategy)

% Displaying images

show_classification(tst.images, q_x, 'AC')

end