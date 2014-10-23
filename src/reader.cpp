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
#include "misc.h"

int main (int argc, char* argv[]){

  const char* dirname;

  if (argc > 1) {
    dirname = argv[1];
  } else {
    dirname = "../data/";
  }

  ccmc::Kameleon kameleon1, kameleon2;
  std::string filename1, filename2;
  int i,j,k;

  int npoinx = 345;
  int npoiny = 187;
  int npoinz = 187;

  float *xcord = new float[npoinx];
  float *ycord = new float[npoiny];
  float *zcord = new float[npoinz];

  grid(xcord,ycord,zcord);

  std::cout << "Reading " << dirname << std::endl;
  std::vector<std::string> chooserun;
  rundirs(&chooserun, dirname);

  int dset1, runi;
  // Loop over run directories
  for (runi = 0; runi < chooserun.size(); runi++) {

    if (runi==3 || runi==7 || runi==11 || runi==15){ // SKIP LFM RUNS
      std::cout << "Skipping Run " << runi << std::endl;
      continue;
    }
    std::cout << "File " << runi << ": " << chooserun[runi] << std::endl;
    dset1 = runi;

    // Get list of timesteps for a given run
    DIR *d;
    d = opendir(dirname);
    struct dirent *dir;
    std::vector<std::string> run1ls;
    std::string dirstring;
    dirstring = dirname;
    dirstring.append(chooserun[dset1]);
    dirstring.append("/GM_CDF");
    d = opendir(dirstring.c_str());
    if (d) {
      while ((dir = readdir(d)) != NULL) {
	if (strstr(dir->d_name,"cdf")) {
	  i++;
	  run1ls.push_back(dir->d_name);
	}
      }
    }
    // SORT THE STRING SO FILENAMES ARE IN ORDER
    std::sort(run1ls.begin(), run1ls.end());

    // Loop over timesteps
    for (int loopnum=0; loopnum<run1ls.size(); loopnum++) {

      filename1 = dirstring;
      filename1.append("/");
      filename1.append(run1ls[loopnum]);
      
      std::cout << "Reading " << filename1 << std::endl;
      long status = kameleon1.open(filename1);
      std::cout << "Opened " << filename1 << " with status: " << status << std::endl;
      std::cout << "ccmc::FileReader::OK = " << ccmc::FileReader::OK << std::endl;
      
      if(kameleon1.doesVariableExist("bz")){kameleon1.loadVariable("bz");}else{return 1;}
      if(kameleon1.doesVariableExist("jx")){kameleon1.loadVariable("jx");}else{return 1;}
      if(kameleon1.doesVariableExist("rho")){kameleon1.loadVariable("rho");}else{return 1;}
      if(kameleon1.doesVariableExist("ux")){kameleon1.loadVariable("ux");}else{return 1;}
      
      // INTERPOLATION
      ccmc::Interpolator * interpolator = kameleon1.createNewInterpolator();
      std::cout << "Interpolating" << std::endl;
      boost::numeric::ublas::matrix<float> value(4,npoinx*npoiny*npoinz);
      int l=0;
      for ( k = 0; k < npoinz; k++){
	for ( j = 0; j < npoiny; j++){
	  for ( i = 0; i < npoinx; i++){
	    value(0,l) = interpolator->interpolate("bz", xcord[i], ycord[j], zcord[k]);
	    value(1,l) = interpolator->interpolate("jx", xcord[i], ycord[j], zcord[k]);
	    value(2,l) = interpolator->interpolate("rho", xcord[i], ycord[j], zcord[k]);
	    value(3,l) = interpolator->interpolate("ux", xcord[i], ycord[j], zcord[k]);
	    l++;
	  }
	}
      }
      
      std::cout << "Done.\nUnloading Variables" << std::endl;
      kameleon1.unloadVariable("bz");
      kameleon1.unloadVariable("jx");
      kameleon1.unloadVariable("rho");
      kameleon1.unloadVariable("ux");
      
      // WRITE BINARY VTK FILE
      std::string resultfilename;
      std::cout << "Starting VTK Write" << std::endl;
      resultfilename = "../data/";
      resultfilename.append(chooserun[dset1]);
      resultfilename.append("/Results");
      resultfilename.append("/Result");
      resultfilename.append(boost::lexical_cast<std::string>(loopnum));
      resultfilename.append(".vtk");

      vtk(resultfilename,xcord,ycord,zcord,value);

      value.clear();
      kameleon1.close();
      
    }
  }
  //delete interpolator;
  return 0;
}
