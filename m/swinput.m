fid = fopen('swinput_150.txt','w');
%2000 1 1 6 0 0 0 0 0 -3 -441.71 0 0 5.76 101289
Bzo = 3.1;
Bzf = -3.0;
for hr=0:7
    for mn = 0:59
        if ((hr*60 + mn) < 150)
            fprintf(fid,'2000 1 1 %d %d 0 0 0 0 %.1f -441.71 0 0 5.76 101289\n',hr,mn,Bzo);
        else
            fprintf(fid,'2000 1 1 %d %d 0 0 0 0 %.1f -441.71 0 0 5.76 101289\n',hr,mn,Bzf);
        end
    end
end
fprintf(fid,'2000 1 1 %d %d 0 0 0 0 3.1 -441.71 0 0 5.76 101289\n',hr+1,0);
fclose(fid);
!head swinput_150.txt
!tail swinput_150.txt