r = 0:.001:5;
R = 0:.001:5;
V = 0.6;

min_r = intmax;
min_R = intmax;
min_S = realmax;

for a = r
    for b = R
        S = povrch(a, b, V);
        if S >= min_S
            continue
        end
        min_r = a;
        min_R = b;
        min_S = S;
    end
end

min_r
min_R
height = (V * 3) ./ ...
    (pi * (min_r.^2 + min_r .* min_R + min_R.^2))
povrch(min_r, min_R, V)