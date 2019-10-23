r = 0:.2:1;
R = r;
v = 0.6;

hold on
for a = r
    y = povrch(a, R, v);
    plot(r, y);
end
hold off

xlabel('R');
ylabel('S');
title('Závislost S na R');
legend('r = 0', 'r = 0.2', 'r = 0.4', 'r = 0.6', 'r = 0.8', 'r = 1');