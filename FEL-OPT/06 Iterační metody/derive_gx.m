function [gxd] = derive_gx(x_points, y_points, center, radius)

length_x = length(x_points);
gxd = zeros(length_x, 3);

for i = 1:length_x
    x = x_points(i);
    y = y_points(i);
    
    distance = norm(center - [x y]);
    distance_minus_radius = distance - radius;
    const_frac = (2 * distance_minus_radius) / distance;
    gxd(i, 1) = const_frac * (center(1) - x);
    gxd(i, 2) = const_frac * (center(2) - y);
    gxd(i, 3) = -2 * distance_minus_radius;
end
