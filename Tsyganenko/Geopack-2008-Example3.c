#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

main (int argc, char *argv[]) {  

  /* Input Position */
  float x=-5.0,y=0.0,z=0.0;

  /* Radial outward in GSM so that GSW system = GSM system */
  float vx=-400.0,vy=0.0,vz=0.0;

  /* Output */
  float bx,by,bz;

  int iUT[5] = {2000,1,1,0,0};

  /* Test */
  recalc_08_(&iUT[0], &iUT[1], &iUT[2], &iUT[3], &iUT[4],&vx,&vy,&vz);
  igrf_gsw_08_(&x,&y,&z,&bx,&by,&bz);
  printf("bx=%f,by=%f,bz=%f\n",bx,by,bz);

  return 0;

}






