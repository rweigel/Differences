void writetxt(std::string resultfilename, boost::numeric::ublas::matrix<float> value, boost::numeric::ublas::matrix<float> xyz, int *s) {

  int npoinx = s[0];
  int npoiny = s[1];
  int npoinz = s[2];

  int i;
  int vn;

  FILE *myfile_cut;
  
  std::string cutfile;
  cutfile = resultfilename;

  myfile_cut = fopen(cutfile.c_str(),"w");

  for (i=0; i<npoinx*npoinz; i++){
    fprintf(myfile_cut,"%f %f %f ",xyz(0,i),xyz(1,i),xyz(2,i));
    for (vn = 0; vn < 14; vn++) {
      fprintf(myfile_cut,"%e ",value(vn,i));
    }
    fprintf(myfile_cut,"\n");
  }
  fclose(myfile_cut);

}

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

void writevtk(std::string resultfilename, float *xcord, float *ycord, float *zcord, boost::numeric::ublas::matrix<float> value, int *s) {


  int npoinx = s[0];
  int npoiny = s[1];
  int npoinz = s[2];

  int i;

  float val,valx,valy,valz;
  FILE *myfile;

  myfile = fopen(resultfilename.c_str(),"w");
  fprintf(myfile, "# vtk DataFile Version 3.0\n");
  fprintf(myfile, "Magnetosphere MHD\nBINARY\n");
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

  if (false) {
  fprintf(myfile, "\nSCALARS Bx FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(3,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nSCALARS By FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(4,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nSCALARS Bz FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(5,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }
  }

  fprintf(myfile, "\nVECTORS B FLOAT 1\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    valx = FloatSwap(value(3,i));
    valy = FloatSwap(value(4,i));
    valz = FloatSwap(value(5,i));
    fwrite((void *)&valx, sizeof(float), 1, myfile);
    fwrite((void *)&valy, sizeof(float), 1, myfile);
    fwrite((void *)&valz, sizeof(float), 1, myfile);
  }

  if (false) {
  fprintf(myfile, "\nSCALARS Jx FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(6,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nSCALARS Jy FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(7,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nSCALARS Jz FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(8,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nVECTORS J FLOAT 1\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    valx = FloatSwap(value(6,i));
    valy = FloatSwap(value(7,i));
    valz = FloatSwap(value(8,i));
    fwrite((void *)&valx, sizeof(float), 1, myfile);
    fwrite((void *)&valy, sizeof(float), 1, myfile);
    fwrite((void *)&valz, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nSCALARS Ux FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(9,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nSCALARS Uy FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(10,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nSCALARS Uz FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(11,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nVECTORS U FLOAT 1\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    valx = FloatSwap(value(9,i));
    valy = FloatSwap(value(10,i));
    valz = FloatSwap(value(11,i));
    fwrite((void *)&valx, sizeof(float), 1, myfile);
    fwrite((void *)&valy, sizeof(float), 1, myfile);
    fwrite((void *)&valz, sizeof(float), 1, myfile);
  }
  }

  fprintf(myfile, "\nSCALARS p FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(12,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fprintf(myfile, "\nSCALARS rho FLOAT 1\n");
  fprintf(myfile, "LOOKUP_TABLE default\n");
  for (i=0; i<npoinx*npoiny*npoinz; i++){
    val = FloatSwap(value(13,i));
    fwrite((void *)&val, sizeof(float), 1, myfile);
  }

  fclose(myfile);
}
