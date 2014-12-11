clear;close all
if exist('colorbarf') ~= 2
    addpath('colorbarf');
end
if exist('hatchfill') ~= 2
    addpath('hatchfillpkg');
end
%  k         1    2    3    4     5     6    7    8    9    10   11   12   13   14   15   16   17  18
vars    = {'xg','yg','zg','xgi','ygi','zgi','bx','by','bz','jx','jy','jz','ux','uy','uz','p','rho','b' };
VarsMax = [ 20 , 100, 100,   20,  100,  100, 100,  10,  50, 1e-3, 1e-2, 1e-4, 500, 50,  200,  2,   30, 400];
VarsMin = [-20 ,-100,-100,  -20, -100, -100,-100, -10, -50,-1e-3,-1e-2,-1e-4,-500,-50, -200,  0,    0,   0];
VarsDel = [ 10,   10,  10,   10,   10,   10,  10,   1,  10, 1e-4, 1e-3, 1e-5, 100, 10,   50,0.2,    2, 100];
VarsNaN = [  0,    0,   0,    0,    0,    0,   0,   2,  10,   0,   0,   0,   0,  0,    0,  0,    0,   0];

Vars    = {'xg','yg','zg','xgi','ygi','zgi','B_x','B_y','B_z','J_x','J_y','J_z','U_x','U_y','U_z','P','N','B'};
VarsU   = {'R_E','R_E','R_E','R_E','R_E','R_E','nT','nT','nT','pA','pA','pA','km/s','km/s','km/s','nPa','cm^{-3}','nT'};

A  = 'Brian_Curtis_042213_1'; % OpenGGCM
Ta = '30';

B  = 'Brian_Curtis_042213_5'; % OpenGGCM
Tb = '90';
slice
break

%B  = 'Brian_Curtis_102114_1'; % OpenGGCM
%Tb = '210';
%slice

A  = 'Brian_Curtis_042213_2'; % BATSRUS
Ta = '30';

B  = 'Brian_Curtis_042213_6'; % BATSRUS
Tb = '90';
slice

B  = 'Brian_Curtis_102114_2'; % BATSRUS
Tb = '210';
slice
