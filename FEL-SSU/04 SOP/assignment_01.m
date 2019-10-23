%% INITIALIZATION

load('ocr_names');

n_features = 8256 + 1; % Features + bias
n_letters = 26;
n_examples = size(TrnData, 2);

% Initialize weight matrix
w = zeros(n_features, n_letters);

number_of_errors = 0;

%% LEARNING

fprintf('\nLearning\n========\n');

epoch = 1;

while 1
    for i = 1:n_examples
        example = TrnData(i);
        
        for j = 1:size(example.X, 2)
            input_data = [example.X(:, j); 1];
            
            % Find max
            max = -Inf;
            max_index = -1;
            for letter_index = 1:size(w, 2)
                features = w(:, letter_index);
                result = dot(features, input_data);
                if result > max
                    max = result;
                    max_index = letter_index;
                end
            end
            
            class = example.Y(j) - 'a' + 1;
    
            % Update
            if max_index ~= class
                w(:, class) = w(:, class) + input_data;
                w(:, max_index) = w(:, max_index) - input_data;
                number_of_errors = number_of_errors + 1;
            end
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
    example = data(i);
    
    e_char = 0;

    for j = 1:size(example.X, 2)
        input_data = [example.X(:, j); 1];

        % Find max
        max = -Inf;
        max_index = -1;
        for letter_index = 1:size(w, 2)
            features = w(:, letter_index);
            result = dot(features, input_data);
            if result > max
                max = result;
                max_index = letter_index;
            end
        end

        class = example.Y(j) - 'a' + 1;
        
        if class ~= max_index
            e_char = e_char + 1;
        end
    end
    
    total_e_char = total_e_char + e_char;
    total_e_seq = total_e_seq + (e_char > 0);
end

total_length = sum(arrayfun(@(i) size(TstData(i).X, 2), 1:size(TstData, 2)));
total_e_char = total_e_char / total_length;
total_e_seq = total_e_seq / size(data, 2);

fprintf('Total R^char error: %.2f %%\n', total_e_char * 100);
fprintf('Total R^seq error: %.2f %%\n', total_e_seq * 100);
