load volby_2017

teziste = sum(T.data, 2) / size(T.data, 2);
body_v_pocatku = T.data - teziste;

A = body_v_pocatku * body_v_pocatku';
[V, D] = eig(A);
dim2 = V(:,end-1:end);
strany = (body_v_pocatku' * dim2);
barvicky = vec2mat(cell2mat(T.color)', 3);
scatter(strany(:,1), strany(:,2), 75, barvicky, 'filled');
saveas(gcf, 'strany.png');