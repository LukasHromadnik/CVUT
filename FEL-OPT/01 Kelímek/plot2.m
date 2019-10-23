r = .1:.01:1;
R = .1:.01:1;

[X, Y] = meshgrid(r, R);
Z = povrch(X, Y, 0.6);
surf(X, Y, Z);
title('Závislost S na r a R');
xlabel('r');
ylabel('R');
zlabel('S');