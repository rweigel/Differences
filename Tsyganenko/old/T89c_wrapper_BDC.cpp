#include <iostream>
#include <endian.h>
#include <dirent.h>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <cstdlib>

main (int argc, char *argv[]) {  

  float PARMOD[10];
  PARMOD[0]= 0.0;
  PARMOD[1]= 0.0;
  PARMOD[2]= 0.0;
  PARMOD[3]=0.0;
  PARMOD[4]=0.0;
  PARMOD[5]=0.0;
  PARMOD[6]=0.0;
  
  int iopt;
  int i,j,k;
  float kp = 2.0;
  float ps = 0.0;
  float bxout,byout,bzout; // THESE NEED TO BE STORED IN MEMORY NOW
  // TIMESTEP LOOP STARTS HERE
  //int m;
  //for(m=0;m<=72;m++){
    // NEED TO CALCULATE Kp HERE
    
    // NEED TO COMPARE Kp RESULT TO iopt AND SET
    if(kp < 0.5){
      iopt = 1;
    }
    else if(kp > 0.5 && kp < 1.5){
      iopt = 2;
    }
    else if(kp > 1.5 && kp < 2.5){
      iopt = 3;
    }
    else if(kp > 2.5 && kp < 3.5){
      iopt = 4;
    }
    else if(kp > 3.5 && kp < 4.5){
      iopt = 5;
    }
    else if(kp > 4.5 && kp < 5.5){
      iopt = 6;
    }
    else if(kp > 5.5){
      iopt = 7;
    }

    // CREATE GRID AND STORE IN MEMORY
    int npoinx = 345, npoiny = 187, npoinz = 187;
    int xmin=-222, ymin=-47, zmin=ymin;
    int xmax=30, ymax=47, zmax=ymax;
    //float *xcord = new float[npoinx];
    float *xcord = malloc(npoinx * sizeof(float));
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
    float *ycord = malloc(npoiny * sizeof(float));
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
    float *zcord = malloc(npoinz * sizeof(float));
    zcord[0] = zmin;
    // Z COORDINATES
    while (zcord[i] < zmax){
    	i++;
        if(zcord[i-1] < -30. || zcord[i-1] >= 30.){ zcord[i] = zcord[i-1]+1.; }
        else if(zcord[i-1] < -8. || zcord[i-1] >= 8.){ zcord[i] = zcord[i-1]+0.5; }
        else if(zcord[i-1] <= 0. || zcord[i-1] >= 0.){ zcord[i] = zcord[i-1]+0.25; } 
    }
    
    // GRID LOOP STARTS HERE
    int l=0;
    float *bx = malloc(npoinx*npoiny*npoinz * sizeof(float));
    float *by = malloc(npoiny*npoiny*npoinz * sizeof(float));
    float *bz = malloc(npoinz*npoiny*npoinz * sizeof(float));
    for ( k = 0; k < npoinz; k++){
	    for ( j = 0; j < npoiny; j++){
		    for ( i = 0; i < npoinx; i++){
		        // EACH NEW GRID POINT PASS THROUGH THIS FUNCTION
			    t89c_(&iopt,PARMOD,&ps,&xcord[i],&ycord[j],&zcord[k],&bxout,&byout,&bzout);
			    bx[l] = bxout;
			    by[l] = byout;
			    bz[l] = bzout;
			    //printf("bx=%f,by=%f,bz=%f\n",bxout,byout,bzout);
			    //return 0;
			    l++;
		    }
	    }
    }
  //}
  
    // WRITE VTK FILE
    std::cout << "Starting VTK Write" << std::endl;
    resultfilename = "/home/bcurtis/Tsyganenko/output/";
	resultfilename.append("/Result");
	resultfilename.append(boost::lexical_cast<std::string>(m));
	resultfilename.append(".vtk");
	std::cout << "Result File Name is: " << resultfilename << std::endl;
	// SPRINTF??????
	
	// BINARY VTK FILE
	float val, val1, val2;
	FILE *myfile;
	myfile = fopen(resultfilename.c_str(),"w");
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
    fprintf(myfile, "\nSCALARS Bx FLOAT 1\n");
    fprintf(myfile, "LOOKUP_TABLE default\n");
    for (i=0; i<npoinx*npoiny*npoinz; i++){
      val = FloatSwap(bx[i]);
      fwrite((void *)&val, sizeof(float), 1, myfile);
    }
    fprintf(myfile, "\nSCALARS By FLOAT 1\n");
    fprintf(myfile, "LOOKUP_TABLE default\n");
    for (i=0; i<npoinx*npoiny*npoinz; i++){
      val = FloatSwap(by[i]);
      fwrite((void *)&val, sizeof(float), 1, myfile);
    }

    fprintf(myfile, "\nSCALARS Bz FLOAT 1\n");
    fprintf(myfile, "LOOKUP_TABLE default\n");
    for (i=0; i<npoinx*npoiny*npoinz; i++){
      val = FloatSwap(bz[i]);
      fwrite((void *)&val, sizeof(float), 1, myfile);
    }
	fclose(myfile);
	std::cout << "VTK Write Complete" << std::endl;
  
  free(xcord); free(bx);
  free(ycord); free(by);
  free(zcord); free(bz);
  return 1;

}






