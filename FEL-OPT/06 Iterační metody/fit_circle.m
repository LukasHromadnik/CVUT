function [x] = fit_circle(method)
load 'points'

plot_graph(x_points, y_points, [], 0);

[x, y] = ginput(1);
center = [x y];
plot_graph(x_points, y_points, center, 0);

[x, y] = ginput(1);
r = norm([x y] - center);
plot_graph(x_points, y_points, center, r);
    
xk = [center, r];
    
waitforbuttonpress;
value = double(get(gcf,'CurrentCharacter'));
mu = 1;

while value ~= 13 % enter
    if value == 32 % space
        
        if method == 'GN'
            gx = arrayfun(@(x, y) dist([x y], xk)^2, x_points, y_points);
            gxd = derive_gx(x_points, y_points, xk(1:2), xk(3));
            vk = inv(gxd' * gxd) * gxd' * gx';
            xk = xk - vk';
        elseif method == 'LM'
            gx = arrayfun(@(x, y) dist([x y], xk)^2, x_points, y_points);
            gxd = derive_gx(x_points, y_points, xk(1:2), xk(3));
            vk = inv(gxd' * gxd + mu * eye(size(gxd, 2))) * gxd' * gx';
            xk_new = xk - vk';
            gx_new = arrayfun(@(x, y) dist([x y], xk_new)^2, x_points, y_points);
            
            if sum(gx_new) < sum(gx)
                mu = mu / 10;
                xk = xk_new;
            else
                mu = mu * 10;
            end
        end
        
        plot_graph(x_points, y_points, [xk(1) xk(2)], xk(3));
    end
    
    waitforbuttonpress;
    value = double(get(gcf, 'CurrentCharacter'));
end

close(gcf);
x = xk;
