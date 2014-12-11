#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "grid.h"

main (int argc, char *argv[]) {  

  int i,j,k,l = 0;

  /* Input Position */
  float vx  = -400.0,vy=0.0,vz=0.0;
  int UT[5] = {2000,1,1,0,0};

  float bx,by,bz;

  float *x = NULL;
  float *y = NULL;
  float *z = NULL;
  int *s = NULL;

  grid(&s,&x,&y,&z);

  recalc_08_(&UT[0],&UT[1],&UT[2],&UT[3],&UT[4],&vx,&vy,&vz);

  for ( j = 0; j < s[1]; j++) {
    if (y[j] != 0.0) {
      continue;
    }
    for ( k = 0; k < s[2]; k++) {
      for ( i = 0; i < s[0]; i++) {
	
	igrf_gsw_08_(&x[i],&y[j],&z[k],&bx,&by,&bz);

	printf("%f %f %f %f %f %f\n",x[i],y[j],z[k],bx,by,bz);

      }
    }
  }

  return 0;



}






