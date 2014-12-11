clear
X = load('data/T89c.txt');

nx = length(unique(X(:,1)));
nz = length(unique(X(:,3)));
x = reshape(X(:,1),nx,nz);
z = reshape(X(:,3),nx,nz);

tmp = reshape(X(:,6),nx,nz);

figure(10)
hp = pcolor(x,z,tmp);
set(hp,'LineStyle','none');
colorbar
axis image
set(gca,'XLim',[-15 15]);
set(gca,'YLim',[-30 30]);
 