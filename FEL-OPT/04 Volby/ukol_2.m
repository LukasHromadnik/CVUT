load volby_2017

teziste = sum(T.data) / size(T.data, 1);

body_v_pocatku = T.data - teziste;

A = body_v_pocatku' * body_v_pocatku;
[V, D] = eig(A);
dim2 = V(:,end-1:end);
otazky = (body_v_pocatku * dim2);

odpovedi = sum(T.data, 2);
barvicky = zeros(size(odpovedi, 1), 3);
for i = 1:size(odpovedi)
    if odpovedi(i) > 0
        barvicky(i, 1) = 1;
    else
        barvicky(i, 3) = 1;
    end
end
scatter(otazky(:,1), otazky(:,2), 75, barvicky);
saveas(gcf, 'otazky.png');
