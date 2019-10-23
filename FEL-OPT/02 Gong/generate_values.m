function [matrix, a, result, values] = generate_values(length)
    values = rand(length, 1);
    matrix = gong(values, length);
    a = matrix \ values;
    result = matrix * a;
end