function C = compute_C(X, y)
    error = 1;
    C = 1;
    y(y == 2) = -1;
    while error > 0
        [w, b, ~] = my_svm(X, y, C);
        model.W = w;
        model.b = b;
        classification = classif_lin_svm(X, model);
        error = sum(classification - y);
        C = C - 0.01;
    end
    C
    C = 0.018;
    error = 1;
    while error > 0
        [w, b, ~] = my_svm(X, y, C);
        model.W = w;
        model.b = b;
        classification = classif_lin_svm(X, model);
        error = sum(classification - y);
        C = C - 0.001;
    end
end
