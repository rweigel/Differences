clear;
if exist('colorbarf') ~= 2
    addpath('colorbarf');
end
A = 'Brian_Curtis_042213_1';
Ta = '30';
B = 'Brian_Curtis_042213_5';
Tb = '90';
%B = 'Brian_Curtis_102114_1';
%Bt = '210';

r = 5.0;
for s = [0:1:359]
    xc(s+1) = r*cosd(s);
    yc(s+1) = r*sind(s);
end

T{3}{1} = ['rev. @ ',Ta,' min'];
T{3}{2} = ['rev. @ ',Tb,' min'];
T{3}{3} = [T{3}{2},' - ',T{3}{1}];
Tc{3}{3} = '\DeltaB_z/B_z';
Tc{3}{2} = 'B_z [nT]';
Tc{3}{1} = 'B_z [nT]';
fl{3}    = 'Bz';
sf(3)    = 1;

T{4}{1} = T{3}{1};
T{4}{2} = T{3}{2};
T{4}{3} = [T{4}{2},' - ',T{4}{1}];
Tc{4}{3} = '\DeltaJ_x/J_x';
Tc{4}{2} = 'J_x [pA]';
Tc{4}{1} = 'J_x [pA]';
fl{4}    = 'Jx';
sf(4)    = 1e-6;

T{5}{1} = T{3}{1};
T{5}{2} = T{3}{2};
T{5}{3} = [T{5}{2},' - ',T{5}{1}];
Tc{5}{3} = '\DeltaN/N';
Tc{5}{2} = 'N [cm^{-3}]';
Tc{5}{1} = 'N [cm^{-3}]';
fl{5}    = 'N';
sf(5)    = 1;

for i = 1:72

    fname{3} = sprintf('data/Precondition/%s_minus_%s/pcdiff_%02d_Y_eq_0.txt',B,A,i-1);
    fname{2} = sprintf('data/%s/Results/Result_%02d_Y_eq_0.txt',B,i-1);
    fname{1} = sprintf('data/%s/Results/Result_%02d_Y_eq_0.txt',A,i-1);
    
    for k = 3:5
    for j = 1:3
        
        if ~exist(fname{j})
            continue;
        end
        X{j} = load(fname{j});
        nx = length(unique(X{j}(:,1)));
        nz = length(unique(X{j}(:,2)));
        x = reshape(X{j}(:,1),nx,nz);
        z = reshape(X{j}(:,2),nx,nz);

        if (k == 3)
            if (j == 3)
                tmp = X{j}(:,k);

                I = find(abs(X{j}(:,3)) < -100);
                tmp(I) = -110;
                I = find(abs(X{j}(:,3)) > 100);
                tmp(I) = 110;
                V = [-110:10:110];
                Nc = (length(V)-1);
                I = find(abs(X{1}(:,3)) < 10 | abs(X{2}(:,3)) < 10);
                tmp(I) = NaN;
                tmp(1) =  110; 
                tmp(2) = -110;
            else
                tmp = X{j}(:,k)/sf(k);

                I = find(X{j}(:,3) > 50);        
                tmp(I) = 60;
                I = find(X{j}(:,3) < -50);        
                tmp(I) = -60;
                V = [-60:10:60];
                Nc = length(V)-1;
                tmp(1) = 60; 
                tmp(2) = -60;
            end
        end

        if (k == 4)
            if (j == 3)
                tmp = X{j}(:,k);

                I = find(abs(X{j}(:,k)) < -100);
                tmp(I) = -110;
                I = find(abs(X{j}(:,k)) > 100);
                tmp(I) = 110;
                V = [-110:10:110];
                Nc = length(V)-1;
                %I = find(abs(X{1}(:,k)) < 10 | abs(X{2}(:,k)) < 10);
                %tmp(I) = NaN;
                tmp(1) =  110; 
                tmp(2) = -110;
            else
                tmp = X{j}(:,k)/sf(k);
                a = 100;
                b = -a;
                I = find(tmp > a);        
                tmp(I) = a;
                I = find(tmp < b);        
                tmp(I) = b;
                dc = 10;
                V = [b-dc:dc:a+dc];
                Nc = length(V)-1;
                tmp(1) = a+dc; 
                tmp(2) = b-dc;
            end
        end

        if (k == 5)
            if (j == 3)
                tmp = X{j}(:,k);

                I = find(abs(X{j}(:,k)) < -100);
                tmp(I) = -110;
                I = find(abs(X{j}(:,k)) > 100);
                tmp(I) = 110;
                V = [-110:10:110];
                Nc = length(V)-1;
                %I = find(abs(X{1}(:,k)) < 10 | abs(X{2}(:,k)) < 10);
                %tmp(I) = NaN;
                tmp(1) =  110; 
                tmp(2) = -110;
            else
                tmp = X{j}(:,k)/sf(k);
                a = 25;
                b = 0;
                I = find(tmp > a);        
                tmp(I) = a;
                dc = 5;
                V = [b-dc:dc:a+dc];
                Nc = length(V)-1;
                tmp(1) = a+dc; 
                tmp(2) = b;
            end
        end

        Bz{j} = reshape(tmp,nx,nz);
        
        figure(j);clf;
        %[c,hc] = contourf(x,z,Bz{j},V);
        hp = pcolor(x,z,Bz{j});
        set(hp,'LineStyle','none')
        axis image
        set(gca,'XLim',[-15 15]);
        set(gca,'YLim',[-30 30]);
        colormap((jet(Nc)))
        hc  = colorbar;
        set(get(hc,'Title'),'String',Tc{k}{j})
        patch(xc,yc,'k');
        xlabel('X [R_E]');
        ylabel('Z [R_E]');
        %colorbarf(c,hc);
        title(sprintf('T=%d [min]; %s',(i-1)*5,T{k}{j}));
        set(gcf,'PaperPosition',[0 0 5.0 7.0])
        set(gcf,'PaperSize',[5.25 7.25])
        %strrep(fname{j},'.txt',['_',fl{k},'.png'])
        print('-dpng',strrep(fname{j},'.txt',['_',fl{k},'.png']));
        print('-depsc',strrep(fname{j},'.txt',['_',fl{k},'.eps']));
    end
    end

end
