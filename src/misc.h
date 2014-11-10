#ifndef ADD_H
#define ADD_H

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

void vtk(std::string resultfilename, float *xcord, float *ycord, float *zcord, boost::numeric::ublas::matrix<float> value, boost::numeric::ublas::matrix<float> xyz) {

  int npoinx = 345;
  int npoiny = 187;
  int npoinz = 187;

  int i;
  float val, val1, val2;
  FILE *myfile;

  FILE *myfile_cut;

  
  std::string cutfile;
  cutfile = resultfilename;
  cutfile.append("_Y_eq_0.txt");

  myfile_cut = fopen(cutfile.c_str(),"w");
  std::cout << "Writing: " << cutfile << std::endl;  
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    if (xyz(1,i) == 0.0) {
      fprintf(myfile_cut,"%f %f %f %f %f %f\n",xyz(0,i),xyz(2,i),value(0,i),value(1,i),value(2,i),value(3,i));
    }
  }
  fclose(myfile_cut);
  std::cout << "Wrote: " << cutfile << std::endl;  


  resultfilename.append(".vtk");

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
  fprintf(myfile, "\nSCALARS Bz FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(0,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }
  fprintf(myfile, "\nSCALARS Jx FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(1,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }
  fprintf(myfile, "\nSCALARS rho FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(2,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }
  fprintf(myfile, "\nSCALARS Ux FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(3,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fclose(myfile);
  
}

void rundirs(std::vector<std::string>* chooserun, const char* dirname) {

  int i;

  // GET THE DIRECTORY CONTENTS AND SORT INTO AN ARRAY
  DIR *d;
  struct dirent *dir;
  d = opendir(dirname);
  i = 0;
  std::cout << "Reading " << dirname << std::endl;
  if (d) {
    while ((dir = readdir(d)) != NULL) {

      if (strcmp(dir->d_name,"..") && strcmp(dir->d_name,".") && strcmp(dir->d_name,"lost+found")){
	i++;
	chooserun->push_back(dir->d_name);
	std::cout << "Found directory " << dir->d_name << std::endl;
      }
    }
  } else {
    std::cout << "Error opening " << dirname << std::endl;
  }

  std::sort(chooserun->begin(), chooserun->end());

}

void grid(float *xcord, float *ycord, float *zcord) {
  // CREATING PERSONALIZED MAGNETOSPHERIC GRID
  int xmin = -222;
  int ymin = -47;
  int zmin = ymin;
  int xmax = 30;
  int ymax = 47;
  int zmax = ymax;
  int i;

  // X COORDINATES
  xcord[0] = xmin;
  i = 0;
  while (xcord[i] < xmax){
    i++;
    if(xcord[i-1] < -30. || xcord[i-1] >= 30.){ xcord[i] = xcord[i-1]+1.; }
    else if(xcord[i-1] < -8. || xcord[i-1] >= 8.){ xcord[i] = xcord[i-1]+0.5; }
    else if(xcord[i-1] <= 0. || xcord[i-1] >= 0.){ xcord[i] = xcord[i-1]+0.25; } 
  }

  // Y COORDINATES
  ycord[0] = ymin;
  i = 0;
  while (ycord[i] < ymax){
    i++;
    if(ycord[i-1] < -30. || ycord[i-1] >= 30.){ ycord[i] = ycord[i-1]+1.; }
    else if(ycord[i-1] < -8. || ycord[i-1] >= 8.){ ycord[i] = ycord[i-1]+0.5; }
    else if(ycord[i-1] <= 0. || ycord[i-1] >= 0.){ ycord[i] = ycord[i-1]+0.25; } 
  }

  // Z COORDINATES
  zcord[0] = zmin;
  i = 0;
  while (zcord[i] < zmax){
    i++;
    if(zcord[i-1] < -30. || zcord[i-1] >= 30.){ zcord[i] = zcord[i-1]+1.; }
    else if(zcord[i-1] < -8. || zcord[i-1] >= 8.){ zcord[i] = zcord[i-1]+0.5; }
    else if(zcord[i-1] <= 0. || zcord[i-1] >= 0.){ zcord[i] = zcord[i-1]+0.25; } 
  }

}

#endif
