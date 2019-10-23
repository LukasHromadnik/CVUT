A = load('walk1.txt','-ASCII');

rank = 2;
[A_approx, base] = approximation(A, rank);
y = base \ A_approx';
plot(y(1,:), y(2,:));
title('Walk trajectory with rank 2');
saveas(gcf, 'walk-r2.png');

rank = 3;
[A_approx, base] = approximation(A, rank);
y = base \ A_approx';
plot3(y(1,:), y(2,:), y(3,:));
title('Walk trajectory with rank 3');
saveas(gcf, 'walk-r3.png');

A = load('makarena1.txt','-ASCII');

rank = 2;
[A_approx, base] = approximation(A, rank);
y = base \ A_approx';
plot(y(1,:), y(2,:));
title('Makarena trajectory with rank 2');
saveas(gcf, 'makarena-r2.png');

rank = 3;
[A_approx, base] = approximation(A, rank);
y = base \ A_approx';
plot3(y(1,:), y(2,:), y(3,:));
title('Makarena trajectory with rank 3');
saveas(gcf, 'makarena-r3.png');
