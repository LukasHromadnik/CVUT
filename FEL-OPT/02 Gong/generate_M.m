function matrix = generate_M(y, p)
    [T, values_width] = size(y);
    width = p + 1;
    height = T - p + 1;
    matrix = zeros(height, width);
    
    for row = 1:height
        matrix(row, 1) = 1;
        
        for col = 2:width
            matrix(row, col) = y((p - col + 2) + row - 1);
        end
    end
end
        