function slice2(A,Ta,B,Tb,off,pd)

if (nargin < 3)
    off = 0;
end
if exist('colorbarf') ~= 2
    addpath('colorbarf');
end
if exist('hatchfill') ~= 2
    addpath('hatchfillpkg');
end 

%  k         1    2    3    4     5     6    7    8    9    10    11    12   13   14   15   16   17  18
vars    = {'xg','yg','zg','xgi','ygi','zgi','bx','by','bz','jx', 'jy', 'jz', 'ux','uy','uz','p','rho','b' };
VarsMax = [ 20 , 100, 100,   20,  100,  100, 100,  10,  50, 1e-3, 1e-2, 1e-4, 500, 50,  200,  2,   30, 400];
VarsMin = [-20 ,-100,-100,  -20, -100, -100,-100, -10, -50,-1e-3,-1e-2,-1e-4,-500,-50, -200,  0,    0,   0];
VarsDel = [ 10,   10,  10,   10,   10,   10,  10,   1,  10, 1e-4, 1e-3, 1e-5, 100, 10,   50,0.2,    2, 100];
VarsNaN = [  0,    0,   0,    0,    0,    0,   0,   0,  0,   0,    0,    0,    0,  0,    0,  0,    0,   0];

Vars    = {'xg','yg','zg','xgi','ygi','zgi','B_x','B_y','B_z','J_x','J_y','J_z','U_x','U_y','U_z','P','N','B'};
VarsU   = {'R_E','R_E','R_E','R_E','R_E','R_E','nT','nT','nT','pA','pA','pA','km/s','km/s','km/s','nPa','cm^{-3}','nT'};

% Create 5 RE circle.
r = 5.0;
for s = [0:1:359]
    xc(s+1) = r*cosd(s);
    yc(s+1) = r*sind(s);
end

for i = 1:72-off
%for i = 1:1

    fname{1} = sprintf('../output/%s/data/cuts/Step_%02d_Y_eq_0.txt',A,i-1);

    if (off > 0)
        fname{2} = sprintf('../output/%s/data/cuts/Step_%02d_Y_eq_0.txt',B,i-1+off);
    end
    
    for k = 7:length(vars)
    %for k = 16:16%length(vars)

        if (off > 0)
            if (pd == 0)
                T{k}{1}  = ['rev. @ ',Tb,' - rev. @',Ta];       
                Tc{k}{1} = ['$\Delta ',Vars{k},'$ [',VarsU{k},']']; 
            else
                T{k}{1}  = ['rev. @ ',Tb,' - rev. @',Ta];
                if (length(Vars{k}) == 3)
                    Tc{k}{1} = ['$\Delta ',Vars{k},'/\overline{',Vars{k}(1),'}',Vars{k}(2:3),'$ [',VarsU{k},']'];                 
                else
                    Tc{k}{1} = ['$\Delta ',Vars{k},'/\overline{',Vars{k}(1),'}','$ [',VarsU{k},']'];                 
                end
            end
        else
            T{k}{1}  = ['rev. @ ',Ta,' min'];       % Figure title
            Tc{k}{1} = ['$',Vars{k},' [',VarsU{k},']$']; % Colorbar title            
        end
            
        
        if (off > 0)
            if (pd == 0)
                fpng = strrep(fname{1},'.txt',['_d',Vars{k},'.png']);
                feps = strrep(fname{1},'.txt',['_d',Vars{k},'.eps']);            
            else
                fpng = strrep(fname{1},'.txt',['_pd',Vars{k},'.png']);
                feps = strrep(fname{1},'.txt',['_pd',Vars{k},'.eps']);                            
            end

            fpng = strrep(fpng,A,['PreconditionDifferences/',B,'_minus_',A]);
            feps = strrep(feps,A,['PreconditionDifferences/',B,'_minus_',A]);
        else
            fpng = strrep(fname{1},'.txt',['_',Vars{k},'.png']);
            feps = strrep(fname{1},'.txt',['_',Vars{k},'.eps']);            
        end
        fpng = strrep(fpng,'data/cuts/','figures/cuts/');
        feps = strrep(feps,'data/cuts/','figures/cuts/');
        
        if ~exist(fileparts(feps),'dir')
            system(sprintf('mkdir -p %s',fileparts(feps)));
        end
        
        fprintf('Loading %s\n',fname{1});
        X{1} = load(fname{1});
        
        if (off > 0)
            fprintf('Loading %s\n',fname{2});
            X{2} = load(fname{2});
        end

        nx = length(unique(X{1}(:,1)));
        nz = length(unique(X{1}(:,3)));
        x = reshape(X{1}(:,1),nx,nz);
        z = reshape(X{1}(:,3),nx,nz);

        % Compute B
        if (k == 18)
            X{1}(:,18) = sqrt(X{1}(:,7).^2+X{1}(:,8).^2+X{1}(:,9).^2);
            if (off > 0)
                X{2}(:,18) = sqrt(X{2}(:,7).^2+X{2}(:,8).^2+X{2}(:,9).^2);
            end
        end

        if (off > 0)
            if (pd == 0)
                tmp = X{2}(:,k)-X{1}(:,k);
            else
                tmp2 = (X{2}(:,k)+X{1}(:,k))/2.0;
                tmp = 100*(X{2}(:,k)-X{1}(:,k))./tmp2;

            end
        else
            tmp = X{1}(:,k);            
        end
        max(tmp)
        min(tmp)
        
        I = find(tmp > VarsMax(k));
        tmp(I) = VarsMax(k)+VarsDel(k);
 
        I = find(tmp < VarsMin(k));
        tmp(I) = VarsMin(k)-VarsDel(k);

        if (off == 0)
            if (VarsMin(k) == 0)
                V = [VarsMin(k):VarsDel(k):VarsMax(k)+VarsDel(k)];
                Nc = length(V)-1;
            else
                V = [VarsMin(k)-VarsDel(k):VarsDel(k):VarsMax(k)+VarsDel(k)];
                Nc = length(V)-1;
            end
            Cm = jet(Nc);

            tmp(1) = V(1);
            tmp(2) = V(end);
        else
            
            if (pd == 1)
                I = find(tmp < -100);
                tmp(I) = -110;
                I = find(tmp > 100);
                tmp(I) = 110;
                V = [-110:10:110];
                Nc = (length(V)-1);

                % If either quantity is small, set difference to NaN
                I = find(abs(X{1}(:,k)) < VarsNaN(k) | abs(X{2}(:,k)) < VarsNaN(k));
                tmp(I) = NaN;

                Cm = jet(22);
                Cm(11:12,:) = 1;
                tmp(1) =  110; 
                tmp(2) = -110;
            else
                mm = max(abs(VarsMin(k)),abs(VarsMax(k)));
                V = [-mm-VarsDel(k):VarsDel(k):mm+VarsDel(k)];
                Nc = length(V)-1;
                Cm = jet(Nc);
                Cm(Nc/2:Nc/2+1,:) = 1;
                tmp(1) = V(1);
                tmp(2) = V(end);
            end
            
        end

        V = reshape(tmp,nx,nz);

        figure(1);
        clf;hold on;
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
        set(get(hc,'Title'),'String',Tc{k}{1},'Interpreter','latex')
        patch(xc,yc,'k');

        xlabel('X [R_E]');
        ylabel('Z [R_E]');
        %colorbarf(c,hc);

        title(sprintf('T=%d [min]; %s',(i-1)*5,T{k}{1}));
        set(gcf,'PaperPosition',[0 0 5.0 7.0])
        set(gcf,'PaperSize',[5.25 7.25])

        com = sprintf('print -dpng %s',fpng);
        eval(com);fprintf('Wrote %s\n',fpng);

        com = sprintf('print -depsc %s',feps);
        eval(com);fprintf('Wrote %s\n',feps);
    end

end
