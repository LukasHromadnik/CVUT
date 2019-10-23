% ee263 layout viewer
% usage:
% view_layout(xfree,yfree,xfixed,yfixed,A)
% plots your layout, asuming A is constructed
% using the free nodes before the fixed ones 
% xfree,yfree and xfixed,yfixed are colum vectors
% the function returns the total square wire length
function f=view_layout(xfree,yfree,xfixed,yfixed,A)

% we have m fixed and n free cells
n = length(xfixed);
m = length(xfree);
k = size(A,1);
figure
hold on
% we plot the paths first
x = [(xfree') (xfixed');(yfree') (yfixed')];

for(i=1:k)
  x1 = x(:,find(A(i,:)==-1));
  x2 = x(:,find(A(i,:)==1));
  plot([x1(1) x2(1)],[x1(2) x2(2)]);
end

% lets plot the fixed,and variable points
plot(xfree,yfree,'kx',xfixed,yfixed,'rx')
hold off
title('Layout')

% we calculate the sum of the squares in this layout
f = 0;
for(i=1:k)
  x1 = x(:,find(A(i,:)==-1));
  x2 = x(:,find(A(i,:)==1));
  f = f ...
    + norm(x1-x2)^2;
end
