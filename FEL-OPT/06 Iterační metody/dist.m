function [d] = dist(a, x)
    c_center = x(1, 1:2);
    d = norm(a - c_center) - x(1, 3);
end