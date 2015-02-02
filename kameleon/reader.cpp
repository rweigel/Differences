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
#include <ccmc/Kameleon.h>
#include <ccmc/FileReader.h>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "grid.h"
#include "export.h"

int main (int argc, char * argv[]){

  std::string dir1string;
  std::string filename1input;
  std::string filename1output;
  std::string tmpstr;

  const char* dir1input = argv[1];
  int mode = atoi(argv[2]);

  DIR *d1;
  struct dirent *dir1struct;
  std::string filename1str;
  std::vector<std::string> run1ls;

  ccmc::Kameleon kameleon1;

  float *x = NULL;
  float *y = NULL;
  float *z = NULL;
  int *s = NULL;

  grid(&s,&x,&y,&z);

  int Npts;
  if (mode == 0) {
    // Write txt file of y=0 plane
    Npts = s[0]*s[2];
  }
  if (mode == 1) {
    // Write vtk of full volume
    Npts = s[0]*s[1]*s[2];
  }
  
  ///////////////////////////////////////////////////////////////    
  //std::cout << "Directory" << ": " << dir1input << std::endl;

  dir1string = dir1input;
  dir1string.append("/GM_CDF");
  std::cout << dir1input << ": Reading." << std::endl;
  d1 = opendir(dir1string.c_str());

  if (d1) {
    while ((dir1struct = readdir(d1)) != NULL) {
      if (strstr(dir1struct->d_name,".cdf")) {
	run1ls.push_back(dir1struct->d_name);
      }
    }
  } else {
    std::cout << "** Could not open " << dir1string << std::endl;
  }
  
  // SORT THE STRING SO FILENAMES ARE IN ORDER
  std::sort(run1ls.begin(), run1ls.end());
  
  std::cout << dir1input << ": Num of files : " << run1ls.size() << std::endl;
  std::cout << dir1input << ": First file   : " << run1ls[0] << std::endl;
  std::cout << dir1input << ": Last file    : " << run1ls[run1ls.size()-1] << std::endl;
  
  ///////////////////////////////////////////////////////////////    

  ///////////////////////////////////////////////////////////////    
  // loop over timesteps
  for(int loopnum = 0; loopnum<run1ls.size(); loopnum++ ){

    filename1input = dir1string;
    filename1input.append("/");
    filename1input.append(run1ls[loopnum]);

    std::cout << dir1input << ": ";
    long status1 = kameleon1.open(filename1input);
    std::cout << filename1input << ": Opened with status: " << status1 << std::endl;

    std::cout << filename1input << ": Loading variables started." << std::endl;
    if(kameleon1.doesVariableExist("x")){kameleon1.loadVariable("x");}else{return 1;}
    if(kameleon1.doesVariableExist("y")){kameleon1.loadVariable("y");}else{return 1;}
    if(kameleon1.doesVariableExist("z")){kameleon1.loadVariable("z");}else{return 1;}
    if(kameleon1.doesVariableExist("bx")){kameleon1.loadVariable("bx");}else{return 1;}
    if(kameleon1.doesVariableExist("by")){kameleon1.loadVariable("by");}else{return 1;}
    if(kameleon1.doesVariableExist("bz")){kameleon1.loadVariable("bz");}else{return 1;}
    if(kameleon1.doesVariableExist("ux")){kameleon1.loadVariable("ux");}else{return 1;}
    if(kameleon1.doesVariableExist("uy")){kameleon1.loadVariable("uy");}else{return 1;}
    if(kameleon1.doesVariableExist("uz")){kameleon1.loadVariable("uz");}else{return 1;}

    if(kameleon1.doesVariableExist("p")) {
      kameleon1.loadVariable("p");
      kameleon1.loadVariable("jx");
      kameleon1.loadVariable("jy");
      kameleon1.loadVariable("jz");
    } else { 
      kameleon1.loadVariable("V_th");
      kameleon1.loadVariable("ei");
      kameleon1.loadVariable("ej");
      kameleon1.loadVariable("ek");
    }
    if(kameleon1.doesVariableExist("rho")){kameleon1.loadVariable("rho");}else{return 1;}
    std::cout << filename1input << ": Loading variables finished." << std::endl;

    ccmc::Interpolator * interpolator1 = kameleon1.createNewInterpolator();
    boost::numeric::ublas::matrix<float> value1(14,Npts);
    boost::numeric::ublas::matrix<float> xyz(3,Npts);

    std::cout << filename1input << ": Interpolating started." << std::endl;
    int l = 0;
    for (int k = 0; k < s[2]; k++){
      for (int j = 0; j < s[1]; j++){

	if ((y[j] != 0.0) & (mode == 0)){
	  continue;
	}

	for (int i = 0; i < s[0]; i++){
	  xyz(0,l) = x[i];
	  xyz(1,l) = y[j];
	  xyz(2,l) = z[k];

	  value1(0,l) = interpolator1->interpolate("x", x[i], y[j], z[k]);
	  value1(1,l) = interpolator1->interpolate("y", x[i], y[j], z[k]);
	  value1(2,l) = interpolator1->interpolate("z", x[i], y[j], z[k]);
	  value1(3,l) = interpolator1->interpolate("bx", x[i], y[j], z[k]);
	  value1(4,l) = interpolator1->interpolate("by", x[i], y[j], z[k]);
	  value1(5,l) = interpolator1->interpolate("bz", x[i], y[j], z[k]);
	  value1(9,l) = interpolator1->interpolate("ux", x[i], y[j], z[k]);
	  value1(10,l) = interpolator1->interpolate("uy", x[i], y[j], z[k]);
	  value1(11,l) = interpolator1->interpolate("uz", x[i], y[j], z[k]);
	  if(kameleon1.doesVariableExist("p")) {
	    value1(6,l) = interpolator1->interpolate("jx", x[i], y[j], z[k]);
	    value1(7,l) = interpolator1->interpolate("jy", x[i], y[j], z[k]);
	    value1(8,l) = interpolator1->interpolate("jz", x[i], y[j], z[k]);
	    value1(12,l) = interpolator1->interpolate("p", x[i], y[j], z[k]);
	  } else {
	    value1(6,l) = interpolator1->interpolate("ei", x[i], y[j], z[k]);
	    value1(7,l) = interpolator1->interpolate("ej", x[i], y[j], z[k]);
	    value1(8,l) = interpolator1->interpolate("ek", x[i], y[j], z[k]);
	    value1(12,l) = interpolator1->interpolate("V_th", x[i], y[j], z[k]);
	  }
	  value1(13,l) = interpolator1->interpolate("rho", x[i], y[j], z[k]);

	  l++;

	}
      }
    }
    std::cout << filename1input << ": Interpolating finished." << std::endl;
    ///////////////////////////////////////////////////////////////    

    kameleon1.close();

    if (mode == 0) {
      ///////////////////////////////////////////////////////////////
      filename1output = dir1input;
      boost::replace_all(filename1output, "data/", "output/");
      filename1output.append("/data/cuts");
      tmpstr = "mkdir -p ";
      tmpstr.append(filename1output);
      system(tmpstr.c_str());
      filename1output.append("/Step_");
      if (loopnum < 10) {
	filename1output.append("0");
      }
      filename1output.append(boost::lexical_cast<std::string>(loopnum));
      filename1output.append("_Y_eq_0.txt");

      std::cout << filename1output << ": Writing started." << std::endl;  
      writetxt(filename1output,value1,xyz,s);
      std::cout << filename1output << ": Writing finished." << std::endl;  
      ///////////////////////////////////////////////////////////////
    }

    if (mode == 1) {
      ///////////////////////////////////////////////////////////////
      filename1output = dir1input;
      boost::replace_all(filename1output, "data/", "output/");
      filename1output.append("/data/volumes");
      tmpstr = "mkdir -p ";
      tmpstr.append(filename1output);
      system(tmpstr.c_str());
      filename1output.append("/Step_");
      if (loopnum < 10) {
	filename1output.append("0");
      }
      filename1output.append(boost::lexical_cast<std::string>(loopnum));
      filename1output.append(".vtk");

      std::cout << filename1output << ": Writing started." << std::endl;  
      writevtk(filename1output,x,y,z,value1,s);
      std::cout << filename1output << ": Writing finished." << std::endl;  

      ///////////////////////////////////////////////////////////////
    }

    value1.clear();

  }
  return 0;
  
}
