load data_A

teziste = sum(A) / size(A, 1);
Atez = A - teziste;
[V, D] = eig(Atez' * Atez);
baze = V(:, 2);
projektor = baze * baze';
nove_body = projektor * Atez';
nove_body_tez = nove_body' + teziste;

hold on
scatter(nove_body_tez(:,1), nove_body_tez(:,2), [], 'red');
scatter(A(:,1),A(:,2), [], 'blue');
hold off
saveas(gcf, 'prolozeni-primkou.png');

soucet_vzdalenosti = D(1,1);
