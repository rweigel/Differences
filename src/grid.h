namespace bnu = boost::numeric::ublas;

bnu::matrix<float> points(char *filename) {

  // Count lines in file
  int N = 0;
  std::ifstream file0(filename); 
  if ( !file0.good() ) {
    std::cout << "Could not open " << filename << "\n";
    std::exit(1);
  }

  std::string line;
  while (getline(file0, line))
    {
      if (!line.empty())
        N++;
    }
  std::cout << "file = " << filename << "; N = " << N << "\n";
  file0.close();

  // Read lines into matrix
  bnu::matrix<float> xyz(N, 3);

  std::ifstream file(filename);
  for(int row = 0; row < N; row++)
    {
      std::string line;
      std::getline(file, line);

      std::stringstream iss(line);

      for (int col = 0; col < 3; col++)
        {
	  std::string val;
	  if (col == 2) {
	    std::getline(iss, val, '\n');
	  } else {
	    std::getline(iss, val,',');
	  }

	  std::stringstream convertor(val);
	  convertor >> xyz(row, col);
        }
      std::cout << "line = " << row;
      std::cout << "; x = " << xyz(row, 0); 
      std::cout << "; y = " << xyz(row, 1);
      std::cout << "; z = " << xyz(row, 2) << "\n";
    }
  file.close();

  return xyz;
}

void grid(int **s,float **xcord, float **ycord, float **zcord) {

  // MAGNETOSPHERIC GRID
  int xmin = -222;
  int ymin = -47;
  int zmin = ymin;
  int xmax = 30;
  int ymax = 47;
  int zmax = ymax;
  int i;

  (*s) = (int*) malloc(3*sizeof(**s));

  (*s)[0] = 345;
  (*s)[1] = 187;
  (*s)[2] = 187;

  (*xcord) = (float*) malloc((*s)[0]*sizeof(**xcord));
  (*ycord) = (float*) malloc((*s)[1]*sizeof(**ycord));
  (*zcord) = (float*) malloc((*s)[2]*sizeof(**zcord));

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
