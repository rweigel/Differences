#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "grid.h"

main (int argc, char *argv[]) {  

  /* Dummy array.  Not used.  See T89c.for */
  float PARMOD[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

  /* Kp level */  
  int iopt = 1;
  
  /* Dipole Tilt */
  float ps = 0.0;

  /* Input Position */
  float x=-5.0,y=0.0,z=0.0;

  /* Output */
  float bx,by,bz;

  /* Test */
  if (argc == 2) {
    t89c_(&iopt,PARMOD,&ps,&x,&y,&z,&bz,&by,&bz);
    printf("bx=%f,by=%f,bz=%f\n",bx,by,bz);

    x=-4.0,y=0.0,z=0.0;
    t89c_(&iopt,PARMOD,&ps,&x,&y,&z,&bz,&by,&bz);
    printf("bx=%f,by=%f,bz=%f\n",bx,by,bz);

    t89c_(&iopt,PARMOD,&ps,&x,&y,&z,&bz,&by,&bz);
    printf("bx=%f,by=%f,bz=%f\n",bx,by,bz);

    x=-3.0,y=0.0,z=0.0;
    t89c_(&iopt,PARMOD,&ps,&x,&y,&z,&bz,&by,&bz);
    printf("bx=%f,by=%f,bz=%f\n",bx,by,bz);

    return 0;
  }

  int npoinx = 345;
  int npoiny = 187;
  int npoinz = 187;

  float *xcord = malloc(npoinx * sizeof(float));
  float *ycord = malloc(npoiny * sizeof(float));
  float *zcord = malloc(npoinz * sizeof(float));

  grid(xcord,ycord,zcord);

  float bxout,byout,bzout;
  int i,j,k,l = 0;

  for ( j = 0; j < npoiny; j++){
    if (ycord[j] != 0.0) {
      continue;
    }
    for ( k = 0; k < npoinz; k++){
      for ( i = 0; i < npoinx; i++){
	
	t89c_(&iopt,PARMOD,&ps,&xcord[i],&ycord[j],&zcord[k],&bxout,&byout,&bzout);
	printf("%f %f %f %f %f %f\n",xcord[i],ycord[j],zcord[k],bxout,byout,bzout);

      }
    }
  }

  return 0;

}






