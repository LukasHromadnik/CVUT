axis equal;
xlim([-15 15]);
ylim([-10 10]);

button = 1;
x_points = [];
y_points = [];

while 1
    [x, y, button] = ginput(1);
    if ~isempty(x)
        x_points(:, end + 1) = x;
    end
    
    if ~isempty(y)
        y_points(:, end + 1) = y;
    end
    
    scatter(x_points, y_points);
    axis equal;
    xlim([-15 15]);
    ylim([-10 10]);
    if isempty(button)
        break
    end
end

save('points.mat', 'x_points', 'y_points');
close(gcf);
