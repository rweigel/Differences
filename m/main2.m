clear;close all;

Runs = {'Brian_Curtis_042213_1',
        'Brian_Curtis_042213_5',
        'Brian_Curtis_102114_1',
        'Brian_Curtis_042213_2',
        'Brian_Curtis_042213_6',
        'Brian_Curtis_102114_2',
        'Brian_Curtis_042213_3',
        'Brian_Curtis_042213_7',
        'Brian_Curtis_102114_3',
        };
%        'Brian_Curtis_042213_4',
%        'Brian_Curtis_042213_8',
%        'Robert_Weigel_020215_1',        
%        };


Tflip = [30,90,210,30,90,210,30,90,210];

if (1)
Runs = {'Brian_Curtis_042213_8'};
off = 0;
for r = 1:1%length(Runs)
    A = Runs{r};
    Ta = num2str(Tflip(r));
    slice2(A,Ta);
end
break
end

if (1)
for pd = 0:1
    for r = [1:3:7]

        A = Runs{r};
        Ta = num2str(Tflip(r));

        B = Runs{r+1};
        Tb = num2str(Tflip(r+1));
        off = 18;
        slice2(A,Ta,B,Tb,off,pd)

        B = Runs{r+2};
        Tb = num2str(Tflip(r+2));
        off = 35;
        slice2(A,Ta,B,Tb,off,pd)
    end
end
break
end

