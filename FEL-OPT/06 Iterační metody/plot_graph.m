function plot_graph(x_points, y_points, center, r)

clf;
hold on;
axis equal;
xlim([-15 15]);
ylim([-10 10]);
scatter(x_points, y_points, [], [204 204 0]/256, 'x');

if ~isempty(center)
    if r > 0
        th = 0:pi/50:2*pi;
        xunit = r * cos(th) + center(1);
        yunit = r * sin(th) + center(2);
        plot(xunit, yunit, 'r');
    end
    
    scatter(center(1), center(2), [], 'r');
end

hold off;

end
