%% INITIALIZATION
load('ocr_names');

n_features = 8256 + 1; % Features + bias
n_examples = size(TrnData, 2);
n_letters = 26;

% Initialize weight matrix
w = zeros(n_features, n_letters);

%% PREPROCESSING

max_name_length = max(arrayfun(@(x) size(TrnData(x).X, 2), 1:n_examples));
names = zeros(max_name_length, n_examples);
for i = 1:n_examples
    example = TrnData(i);
    length = size(example.X, 2);
    j = 0;
    found = 0;
    for j = 1:n_examples
        example_index = names(length, j);
        if example_index == 0
            break
        end
        if sum(TrnData(example_index).Y == example.Y) == length
            found = 1;
            break
        end
    end
    if found == 0
        names(length, j) = i;
    end
end

% Trim the matrix
% Unnecessary, can be removed
max_column = 0;
for i = 1:max_name_length
    for j = 1:n_examples
        if names(i, j) == 0
            if (j - 1) > max_column
                max_column = j - 1;
            end
            break
        end
    end
end
names = names(:, 1:max_column);

frequency = zeros(size(names));
number_of_errors = 0;

%% LEARNING

fprintf('\nLearning\n========\n');

epoch = 1;

while 1
    % All examples
    for i = 1:n_examples
        
        % Load example
        example = TrnData(i);
        
        % Set the length of the name
        length = size(example.X, 2);
        
        % Helper variables
        max_probability = -Inf;
        selected_name = 0;
        
        % Iterate over all names given length
        for j = 1:size(names(length, :), 2)
            
            % No more examples, break it
            if names(length, j) == 0
                break
            end
            
            % Load the name
            name = TrnData(names(length, j)).Y - 'a' + 1;
            
            % Dot product
            dot_sum = 0;
            for k = 1:length
                input_data = [example.X(:, k); 1];
                weight = w(:, name(k));
                result = dot(weight, input_data);
                dot_sum = dot_sum + result;
            end
            dot_sum = dot_sum + frequency(length, j);
            
            if dot_sum >= max_probability
                max_probability = dot_sum;
                selected_name = j;
            end
        end
        
        e_name_index = names(length, selected_name);
        e_name = TrnData(e_name_index).Y - 'a' + 1;
        name = example.Y - 'a' + 1;
        
        if sum(e_name == name) ~= length
            for j = 1:length
                input_data = [example.X(:, j); 1];
                w(:, name(j)) = w(:, name(j)) + input_data;
                w(:, e_name(j)) = w(:, e_name(j)) - input_data;
            end
            
            frequency(length, selected_name) = frequency(length, selected_name) - 1;
            for j = 1:size(names(length, :), 2)
                if sum((TrnData(names(length, j)).Y - 'a' + 1) == (example.Y - 'a' + 1)) == length
                    frequency(length, j) = frequency(length, j) + 1;
                    break
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
    % Load example
    example = data(i);
    example_name = example.Y - 'a' + 1;

    % Set the length of the name
    length = size(example.X, 2);

    % Helper variables
    max_probability = -Inf;
    selected_name = 0;

    % Iterate over all names given length
    for j = 1:size(names(length, :), 2)

        % No more examples, break it
        if names(length, j) == 0
            break
        end

        % Load the name
        name = TrnData(names(length, j)).Y - 'a' + 1;

        % Dot product
        dot_sum = 0;
        for k = 1:length
            input_data = [example.X(:, k); 1];
            weight = w(:, name(k));
            result = dot(weight, input_data);
            dot_sum = dot_sum + result;
        end
        dot_sum = dot_sum + frequency(length, j);

        if dot_sum >= max_probability
            max_probability = dot_sum;
            selected_name = j;
        end
    end
    
    best_name = TrnData(names(length, selected_name)).Y - 'a' + 1;
    
    e_char = sum(best_name ~= example_name);
    total_e_seq = total_e_seq + (e_char > 0);
    total_e_char = total_e_char + e_char;
end

total_length = sum(arrayfun(@(i) size(TstData(i).X, 2), 1:size(TstData, 2)));
total_e_char = total_e_char / total_length;
total_e_seq = total_e_seq / size(data, 2);

fprintf('Total R^char error: %.2f %%\n', total_e_char * 100);
fprintf('Total R^seq error: %.2f %%\n', total_e_seq * 100);
