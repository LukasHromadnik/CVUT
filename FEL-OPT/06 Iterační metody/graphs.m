r = 1;
[x, y] = meshgrid(0:.05:5);
%points = [2 2];
points = [2 2; 2 4];
%points = rand(20, 2) * 3;

result = zeros();
for i = 1:size(points, 1)
    result = result + (sqrt((x - points(i, 1)).^2 + (y - points(i, 2)).^2) - r).^2;
end

mesh(x, y, result);
