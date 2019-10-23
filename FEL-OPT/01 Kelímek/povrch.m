function out = povrch(r, R, V)
    height = (V * 3) ./ (pi * (r.^2 + r .* R + R.^2));
    podstava = pi * min(min(r), min(R)).^2;
    plast = pi * (r + R) .* sqrt(height.^2 + (r - R).^2);
    out = podstava + plast;
end
