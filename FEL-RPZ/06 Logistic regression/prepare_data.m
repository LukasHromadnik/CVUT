function data = prepare_data(input)
    data = [ones(1, length(input)); compute_measurements(input)];
end