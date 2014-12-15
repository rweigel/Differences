% Create 5 RE circle.
r = 5.0;
for s = [0:1:359]
    xc(s+1) = r*cosd(s);
    yc(s+1) = r*sind(s);
end

for i = 1:72

    fname{1} = sprintf('../data/%s/Results/Result_%02d_Y_eq_0.txt',A,i-1);
    fname{2} = sprintf('../data/%s/Results/Result_%02d_Y_eq_0.txt',B,i-1);
    fname{3} = sprintf('../data/Precondition/%s_minus_%s/pcdiff_%02d_Y_eq_0.txt',B,A,i-1);
    
    for k = 7:length(vars)

        T{k}{1} = ['rev. @ ',Ta,' min'];
        T{k}{2} = ['rev. @ ',Tb,' min'];
        T{k}{3} = [T{k}{2},' - ',T{k}{1}];
        T{k}{4} = T{k}{3};
        Tc{k}{4} = ['\Delta',Vars{k}];
        Tc{k}{3} = ['\Delta',Vars{k},'/',Vars{k}];
        Tc{k}{2} = [Vars{k},' [',VarsU{k},']'];
        Tc{k}{1} = Tc{k}{2};
        fl{k}    = Vars{k};
        
        for j = 1:4

            if (j == 4)
                fpng = strrep(fname{3},'.txt',['_',fl{k},'.png']);
                feps = strrep(fname{3},'.txt',['_',fl{k},'.eps']);  
                fpng = strrep(fpng,'pcdiff','diff');
                feps = strrep(feps,'pcdiff','diff');
                X{j} = X{2} - X{1};
            else
                if ~exist(fname{1}) || ~exist(fname{2})
                    continue; % Difference files have less than 72 time steps.
                end
                fpng = strrep(fname{j},'.txt',['_',fl{k},'.png']);
                feps = strrep(fname{j},'.txt',['_',fl{k},'.eps']);
                if exist(fpng) && exist(feps)
                    fprintf('Files\n  %s\nand\n  %s\nexist.\n',fpng,feps);
                    %continue;
                end
                fprintf('Loading %s\n',fname{j});
                X{j} = load(fname{j});
            end
                
            nx = length(unique(X{1}(:,1)));
            nz = length(unique(X{1}(:,3)));
            x = reshape(X{1}(:,1),nx,nz);
            z = reshape(X{1}(:,3),nx,nz);

            if (k == 18)
                X{j}(:,18) = sqrt(X{j}(:,7).^2+X{j}(:,8).^2+X{j}(:,9).^2);
            end
            
            if (j == 3)
                tmp = X{j}(:,k);

                I = find(abs(X{j}(:,k)) < -100);
                tmp(I) = -110;
                I = find(abs(X{j}(:,k)) > 100);
                tmp(I) = 110;
                V = [-110:10:110];
                Nc = (length(V)-1);
                I = find(abs(X{1}(:,k)) < VarsNaN(k) | abs(X{2}(:,k)) < VarsNaN(k));
                tmp(I) = NaN;
                tmp(1) =  110; 
                tmp(2) = -110;
                Cm = jet(22);
                Cm(11:12,:) = 1;
            else
                tmp = X{j}(:,k);
                I = find(X{j}(:,k) > VarsMax(k));        
                tmp(I) = VarsMax(k)+VarsDel(k);
                I = find(X{j}(:,k) < VarsMin(k));        
                tmp(I) = VarsMin(k)-VarsDel(k);
                if (j == 4)
                    a = max(abs(VarsMin(k)),abs(VarsMax(k)));
                    V = [-a-VarsDel(k):VarsDel(k):a+VarsDel(k)];
                elseif (VarsMin(k) == 0)
                    V = [VarsMin(k):VarsDel(k):VarsMax(k)+VarsDel(k)];
                    Nc = length(V)-1;
                else
                    V = [VarsMin(k)-VarsDel(k):VarsDel(k):VarsMax(k)+VarsDel(k)];
                    Nc = length(V)-1;
                end
                Cm = jet(Nc);
                tmp(1) = V(1); 
                tmp(2) = V(end);
            end

            V = reshape(tmp,nx,nz);

            figure(j);clf;hold on;
            %[c,hc] = contourf(x,z,Bz{j},V);
            p1 = patch([-15,15,15,-15],[-30 -30 30 30],[0.75,0.75,0.75]);
            set(p1,'ZData',[-1 -1 -1 -1])
            h1 = hatchfill(p1);
            XData = get(h1,'XData');
            set(h1,'ZData',-ones(1,length(XData)));
            hp = pcolor(x,z,V);
            set(hp,'LineStyle','none')
            axis image
            set(gca,'XLim',[-15 15]);
            set(gca,'YLim',[-30 30]);
            colormap(Cm)
            hc  = colorbar;
            set(get(hc,'Title'),'String',Tc{k}{j})
            patch(xc,yc,'k');

            xlabel('X [R_E]');
            ylabel('Z [R_E]');
            %colorbarf(c,hc);

            title(sprintf('T=%d [min]; %s',(i-1)*5,T{k}{j}));
            set(gcf,'PaperPosition',[0 0 5.0 7.0])
            set(gcf,'PaperSize',[5.25 7.25])

            com = sprintf('print -dpng %s',fpng);
            eval(com);fprintf('Wrote %s\n',fpng);

            com = sprintf('print -depsc %s',feps);
            eval(com);fprintf('Wrote %s\n',feps);
        end
    end

end
