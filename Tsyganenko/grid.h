void grid(int **s,float **xcord, float **ycord, float **zcord) {

  // CREATING PERSONALIZED MAGNETOSPHERIC GRID
  int xmin = -222;
  int ymin = -47;
  int zmin = ymin;
  int xmax = 30;
  int ymax = 47;
  int zmax = ymax;
  int i;

  (*s) = malloc(3*sizeof(**s));

  (*s)[0] = 345;
  (*s)[1] = 187;
  (*s)[2] = 187;

  (*xcord) = malloc((*s)[0]*sizeof(**xcord));
  (*ycord) = malloc((*s)[1]*sizeof(**ycord));
  (*zcord) = malloc((*s)[2]*sizeof(**zcord));

  // X COORDINATES
  (*xcord)[0] = xmin;
  i = 0;
  while ((*xcord)[i] < xmax){
    i++;
    if((*xcord)[i-1] < -30. || (*xcord)[i-1] >= 30.){ (*xcord)[i] = (*xcord)[i-1]+1.; }
    else if((*xcord)[i-1] < -8. || (*xcord)[i-1] >= 8.){ (*xcord)[i] = (*xcord)[i-1]+0.5; }
    else if((*xcord)[i-1] <= 0. || (*xcord)[i-1] >= 0.){ (*xcord)[i] = (*xcord)[i-1]+0.25; } 
  }

  // Y COORDINATES
  (*ycord)[0] = ymin;
  i = 0;
  while ((*ycord)[i] < ymax){
    i++;
    if((*ycord)[i-1] < -30. || (*ycord)[i-1] >= 30.){ (*ycord)[i] = (*ycord)[i-1]+1.; }
    else if((*ycord)[i-1] < -8. || (*ycord)[i-1] >= 8.){ (*ycord)[i] = (*ycord)[i-1]+0.5; }
    else if((*ycord)[i-1] <= 0. || (*ycord)[i-1] >= 0.){ (*ycord)[i] = (*ycord)[i-1]+0.25; } 
  }

  // Z COORDINATES
  (*zcord)[0] = zmin;
  i = 0;
  while ((*zcord)[i] < zmax){
    i++;
    if((*zcord)[i-1] < -30. || (*zcord)[i-1] >= 30.){ (*zcord)[i] = (*zcord)[i-1]+1.; }
    else if((*zcord)[i-1] < -8. || (*zcord)[i-1] >= 8.){ (*zcord)[i] = (*zcord)[i-1]+0.5; }
    else if((*zcord)[i-1] <= 0. || (*zcord)[i-1] >= 0.){ (*zcord)[i] = (*zcord)[i-1]+0.25; } 
  }

}
