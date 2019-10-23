%% INITIALIZATION

load('ocr_names');

n_features = size(TrnData(1).X, 1) + 1; % Features + bias
n_letters = 26;
n_examples = size(TrnData, 2);

% Initialize weight matrix
w = zeros(n_features, n_letters);
g = zeros(n_letters, n_letters);
number_of_errors = 0;

%% LEARNING

fprintf('\nLearning\n========\n');
epoch = 1;

while 1
    for i = 1:n_examples
        example = TrnData(i);
        length = size(example.X, 2);
        example_name = example.Y - 'a' + 1;
        
        % Compute matrices
        predecessors = zeros(n_letters, length - 1);
        max_cost = zeros(n_letters);
        
        % Get best path
        for j = 2:length
            % Compute dot product with previous layer
            input_data = [example.X(:, j - 1); 1];
            costs = arrayfun(@(x) dot(w(:, x), input_data), 1:n_letters);
            
            % Iterate over letters in current layer
            for k = 1:n_letters
                current_max = -Inf;
                for l = 1:n_letters
                    cost = costs(l) + g(l, k);
                    if cost > current_max
                        current_max = cost;
                        predecessors(k, j - 1) = l;
                    end
                end
                max_cost(k) = current_max;
            end
        end
        
        for j = 1:n_letters
           input_data = [example.X(:, length); 1];
           max_cost(j) = max_cost(j) + dot(w(:, j), input_data);
        end

        % Find last letter
        current_max = -Inf;
        current_index = 0;
        for j = 1:n_letters
            if max_cost(j) > current_max
                current_max = max_cost(j);
                current_index = j;
            end
        end
        
        % Reconstruct path
        path = zeros(1, length);
        path(end) = current_index;
        for j = 1:length - 1
            path(length - j) = predecessors(current_index, length - j);
            current_index = predecessors(current_index, length - j);
        end
        
        % Update
        if sum(path == example_name) ~= length
            for j = 1:length
                input_data = [example.X(:, j); 1];
                w(:, example_name(j)) = w(:, example_name(j)) + input_data;
                w(:, path(j)) = w(:, path(j)) - input_data;
                if j < length
                    g(example_name(j), example_name(j + 1)) = g(example_name(j), example_name(j + 1)) + 1;                     
                    g(path(j), path(j + 1)) = g(path(j), path(j + 1)) - 1;
                end
            end
            number_of_errors = number_of_errors + 1;
        end
    end
    
    fprintf('Epoch %d, number of errors: %d\n', epoch, number_of_errors);
    epoch = epoch + 1;
    
    % Check error
    if number_of_errors == 0
        break
    end
    number_of_errors = 0;
end

%% EVALUATION

fprintf('\nEvaluation\n==========\n');

data = TstData;

total_e_char = 0;
total_e_seq = 0;

for i = 1:size(data, 2)
    example = TstData(i);
    length = size(example.X, 2);
    example_name = example.Y - 'a' + 1;

    % Compute matrices
    predecessors = zeros(n_letters, length - 1);
    max_cost = zeros(n_letters);

    % Get best path
    for j = 2:length
        % Compute dot product with previous layer
        input_data = [example.X(:, j - 1); 1];
        costs = arrayfun(@(x) dot(w(:, x), input_data), 1:n_letters);

        % Iterate over letters in current layer
        for k = 1:n_letters
            current_max = -Inf;
            for l = 1:n_letters
                cost = costs(l) + g(l, k);
                if cost > current_max
                    current_max = cost;
                    predecessors(k, j - 1) = l;
                end
            end
            max_cost(k) = current_max;
        end
    end

    for j = 1:n_letters
       input_data = [example.X(:, length); 1];
       max_cost(j) = max_cost(j) + dot(w(:, j), input_data);
    end

    % Find last letter
    current_max = -Inf;
    current_index = 0;
    for j = 1:n_letters
        if max_cost(j) > current_max
            current_max = max_cost(j);
            current_index = j;
        end
    end

    % Reconstruct path
    path = zeros(1, length);
    path(end) = current_index;
    for j = 1:length - 1
        path(length - j) = predecessors(current_index, length - j);
        current_index = predecessors(current_index, length - j);
    end
    
    e_char = sum(path ~= example_name);
    total_e_seq = total_e_seq + (e_char > 0);
    total_e_char = total_e_char + e_char;
end

total_length = sum(arrayfun(@(i) size(TstData(i).X, 2), 1:size(TstData, 2)));
total_e_char = total_e_char / total_length;
total_e_seq = total_e_seq / size(data, 2);

fprintf('Total R^char error: %.2f %%\n', total_e_char * 100);
fprintf('Total R^seq error: %.2f %%\n', total_e_seq * 100);
