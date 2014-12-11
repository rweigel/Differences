#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

float FloatSwap( float f )
{
   union
   {
      float f;
      unsigned char b[4];
   } dat1, dat2;

   dat1.f = f;
   dat2.b[0] = dat1.b[3];
   dat2.b[1] = dat1.b[2];
   dat2.b[2] = dat1.b[1];
   dat2.b[3] = dat1.b[0];
   return dat2.f;
}

main (int argc, char *argv[]) {  

    float PARMOD[10];
    PARMOD[0]= 5.76;//IMF pressure pdyn
    //PARMOD[1]= -10.0;//Dst
    PARMOD[2]=0.0;//ByIMF
    //PARMOD[3]=-3.0;//BzIMF
    PARMOD[4]=0.0;
    PARMOD[5]=0.0;
    PARMOD[6]=0.0;

    int iopt=1;// DUMMY VARIABLE
    int i,j,k,l,m;
    float ps = 0.0;
    float *xcord, *ycord, *zcord;
    float bxout,byout,bzout; // THESE NEED TO BE STORED IN MEMORY NOW

    float Coeff[48];
//float Coeff_raw[48] = {0.0002, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0002, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0002, 0.0001, 0.0002, 0.0002, 0.0002, 0.0002, 0.0001, 0.0001, 0.0001, 0.0001, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0003, 0.0003, 0.0002, 0.0003, 0.0003, 0.0004, 0.0004, 0.0004, 0.0004, 0.0006, 0.0006, 0.0012, 0.0016, 0.0041};
    float Coeff_raw[48] = {-0.0009, -0.0004, -0.0004, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0003, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0005, -0.0005, -0.0005, -0.0005, -0.0005, -0.0006, -0.0006, -0.0006, -0.0007, -0.0007, -0.0007, -0.0007, -0.0007, -0.0008, -0.0009, -0.0011, -0.0014, -0.0016, -0.0016, -0.0015, -0.0019, -0.0029, -0.0041};
    for(i=0; i<48; i++){
        Coeff[i] = Coeff_raw[47-i];
    }
  
  // GRID CREATION
    int npoinx = 345, npoiny = 187, npoinz = 187;
    int xmin=-222, ymin=-47, zmin=ymin;
    int xmax=30, ymax=47, zmax=ymax;
    //float *xcord = new float[npoinx];
    xcord = (float *) malloc(npoinx * sizeof(float));
    xcord[0] = xmin;
    // X COORDINATES
    i = 0;
    while (xcord[i] < xmax){
        i++;
        if(xcord[i-1] < -30. || xcord[i-1] >= 30.){ xcord[i] = xcord[i-1]+1.; }
        else if(xcord[i-1] < -8. || xcord[i-1] >= 8.){ xcord[i] = xcord[i-1]+0.5; }
        else if(xcord[i-1] <= 0. || xcord[i-1] >= 0.){ xcord[i] = xcord[i-1]+0.25; } 
    }
    i = 0;
    //float *ycord = new float[npoiny];
    ycord = (float *) malloc(npoiny * sizeof(float));
    ycord[0] = ymin;
    // Y COORDINATES
    while (ycord[i] < ymax){
        i++;
        if(ycord[i-1] < -30. || ycord[i-1] >= 30.){ ycord[i] = ycord[i-1]+1.; }
        else if(ycord[i-1] < -8. || ycord[i-1] >= 8.){ ycord[i] = ycord[i-1]+0.5; }
        else if(ycord[i-1] <= 0. || ycord[i-1] >= 0.){ ycord[i] = ycord[i-1]+0.25; } 
    }
    i = 0;
    //float *zcord = new float[npoinz];
    zcord = (float *) malloc(npoinz * sizeof(float));
    zcord[0] = zmin;
    // Z COORDINATES
    while (zcord[i] < zmax){
        i++;
        if(zcord[i-1] < -30. || zcord[i-1] >= 30.){ zcord[i] = zcord[i-1]+1.; }
        else if(zcord[i-1] < -8. || zcord[i-1] >= 8.){ zcord[i] = zcord[i-1]+0.5; }
        else if(zcord[i-1] <= 0. || zcord[i-1] >= 0.){ zcord[i] = zcord[i-1]+0.25; } 
    }
  
  
  // TIMESTEP LOOP STARTS HERE
    float dst, Bs;
    float Vx = -400.0;
    for(m=0;m<=72;m++){
    	printf("Sarting Loop Number (m): %i\n",m);
    //for(m=0;m<=0;m++){
        // WHAT IS BZ (FOR PARMOD) ?
        if(m<6){
            PARMOD[3]=3.0;
        }
        else{
            PARMOD[3]=-3.0;
        }
        // NEED TO CALCULATE Dst HERE
	    dst = 0;
	    for(i = 0; i<48; i++){
		    if(m < 7){ // IRF uses previous Bs values
			    Bs = 0.0;
		    }
		    else{
			    if(i<=m-7){
				    Bs = -3.0;
			    }
			    else{
				    Bs = 0.0;
			    }
		    }
		    printf("Bs: %f\t",Bs);
		    dst += Coeff[i]*Vx*Bs;
		    printf("dst: %f\n",dst);
		    PARMOD[1]=dst;
	    }
	    
        float *bx, *by, *bz;
        bx = (float*)malloc(npoinx*npoiny*npoinz * sizeof(float));
        by = (float*)malloc(npoinx*npoiny*npoinz * sizeof(float));
        bz = (float*)malloc(npoinx*npoiny*npoinz * sizeof(float));
        l=0;
        for ( k = 0; k < npoinz; k++){
	        for ( j = 0; j < npoiny; j++){
		        for ( i = 0; i < npoinx; i++){
		            // EACH NEW GRID POINT PASS THROUGH THIS FUNCTION
		            //t96_01__(&iopt,PARMOD,&ps,&x,&y,&z,&bz,&by,&bz);
		            t96_01__(&iopt,PARMOD,&ps,&xcord[i],&ycord[j],&zcord[k],&bxout,&byout,&bzout);
			        //t89c_(&iopt,PARMOD,&ps,&xcord[i],&ycord[j],&zcord[k],&bxout,&byout,&bzout);
			        bx[l] = bxout;
			        by[l] = byout;
			        bz[l] = bzout;
			        //printf("bx=%f,by=%f,bz=%f\n",bxout,byout,bzout);
			        //return 0;
			        l++;
		        }
	        }
        }

      
        // WRITE VTK FILE
	    // BINARY VTK FILE
	    float val, val1, val2;
	    FILE *myfile;
	    char MyString[90];
	    sprintf(MyString, "/home/bcurtis/Tsyganenko/output/T96/Result%i.vtk",m);
	    printf("Writing %s\n",MyString);
	    myfile = fopen(MyString,"w");
	    fprintf(myfile, "# vtk DataFile Version 3.0\n");
	    fprintf(myfile, "Brian's Data\nBINARY\n");
	    fprintf(myfile, "DATASET RECTILINEAR_GRID\n");
	    fprintf(myfile, "DIMENSIONS %d %d %d\n", npoinx, npoiny, npoinz);
	    fprintf(myfile, "X_COORDINATES %d float\n", npoinx);
	    for (i=0; i<npoinx; i++){
		    val = FloatSwap(xcord[i]);
		    fwrite((void *)&val, sizeof(float), 1, myfile);
	    }
	    fprintf(myfile, "\nY_COORDINATES %d float\n", npoiny);
	    for (i=0; i<npoiny; i++){
		    val = FloatSwap(ycord[i]);
		    fwrite((void *)&val, sizeof(float), 1, myfile);
	    }
	    fprintf(myfile, "\nZ_COORDINATES %d float\n", npoinz);
	    for (i=0; i<npoinz; i++){
		    val = FloatSwap(zcord[i]);
		    fwrite((void *)&val, sizeof(float), 1, myfile);
	    }
	    fprintf(myfile, "\nPOINT_DATA  %d\n", npoinx*npoiny*npoinz);
        //fprintf(myfile, "\nSCALARS Bx FLOAT 1\n");
        fprintf(myfile, "\nVECTORS B FLOAT\n");
        for (i=0; i<npoinx*npoiny*npoinz; i++){
          val = FloatSwap(bx[i]);
          val1 = FloatSwap(by[i]);
          val2 = FloatSwap(bz[i]);
          fwrite((void *)&val, sizeof(float), 1, myfile);
          fwrite((void *)&val1, sizeof(float), 1, myfile);
          fwrite((void *)&val2, sizeof(float), 1, myfile);
        }
        printf("Wrote %s\n", MyString);
        fclose(myfile);
        
        free(bx);
        free(by);
        free(bz);	
  }
  printf("VTK Write Complete\n");	
	/*FILE *bxfile;
	FILE *byfile;
	FILE *bzfile;
	bxfile = fopen("/home/bcurtis/Tsyganenko/output/T96/Bx.dat","w");
	byfile = fopen("/home/bcurtis/Tsyganenko/output/T96/By.dat","w");
	bzfile = fopen("/home/bcurtis/Tsyganenko/output/T96/Bz.dat","w");
	fprintf(bxfile, "X\tY\tZ\tBx\n");
	fprintf(byfile, "X\tY\tZ\tBy\n");
	fprintf(bzfile, "X\tY\tZ\tBz\n");
	l=0;
	for ( k = 0; k < npoinz; k++){
	    for ( j = 0; j < npoiny; j++){
		    for ( i = 0; i < npoinx; i++){
		        fprintf(bxfile, "%f\t%f\t%f\t%f\n", xcord[i], ycord[j], zcord[k], bx[l]);
	            fprintf(byfile, "%f\t%f\t%f\t%f\n", xcord[i], ycord[j], zcord[k], by[l]);
	            fprintf(bzfile, "%f\t%f\t%f\t%f\n", xcord[i], ycord[j], zcord[k], bz[l]);
			    l++;
		    }
	    }
    }
    fclose(bxfile);
    fclose(byfile);
    fclose(bzfile);
    printf(".dat File Write Complete\n");
	*/
  
  free(xcord);
  free(ycord);
  free(zcord);
return 0;
}






