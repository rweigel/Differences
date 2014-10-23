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

int main (int argc, char * argv[]){

  const char* dirname;

  if (argc > 1) {
    dirname = argv[1];
  } else {
    dirname = "../data/";
  }

  const char* dir1s;
  const char* dir2s;

  dir1s = "Brian_Curtis_042213_1";
  dir2s = "Brian_Curtis_042213_5";

  DIR *d1;
  DIR *d2;

  struct dirent *dir1;
  struct dirent *dir2;

  ccmc::Kameleon kameleon1;
  ccmc::Kameleon kameleon2;

  std::string filename1;
  std::string filename2;

  int npoinx = 345;
  int npoiny = 187;
  int npoinz = 187;

  float *xcord = new float[npoinx];
  float *ycord = new float[npoiny];
  float *zcord = new float[npoinz];

  grid(xcord,ycord,zcord);

  std::vector<std::string> chooserun;
  rundirs(&chooserun, dirname);
  
  ///////////////////////////////////////////////////////////////    
  std::cout << "Dataset 1" << ": " << dir1s << std::endl;
  std::cout << "Dataset 2" << ": " << dir2s << std::endl;
  
  std::vector<std::string> run1ls;
  std::string dirstring1;
  dirstring1 = dirname; 
  dirstring1.append(dir1s);
  dirstring1.append("/GM_CDF");
  std::cout << "Reading " << dirstring1 << std::endl;
  d1 = opendir(dirstring1.c_str());

  if (d1) {
    while ((dir1 = readdir(d1)) != NULL) {
      if (strstr(dir1->d_name,"cdf")) {
	run1ls.push_back(dir1->d_name);
      }
    }
  } else {
    std::cout << "Could not open " << dirstring1 << std::endl;
  }
  
  // SORT THE STRING SO FILENAMES ARE IN ORDER
  std::sort(run1ls.begin(), run1ls.end());
  run1ls.erase(run1ls.begin()+32,run1ls.end());
  
  std::cout << "run1ls size: " << run1ls.size() << std::endl;
  std::cout << "First file: " << run1ls[0] << std::endl;
  std::cout << "Last file: " << run1ls[run1ls.size()-1] << std::endl;
  
  std::vector<std::string> run2ls;
  std::string dirstring2;
  dirstring2 = dirname;
  dirstring2.append(dir2s);
  dirstring2.append("/GM_CDF");
  d2 = opendir(dirstring2.c_str());

  if (d2) {
    while ((dir2 = readdir(d2)) != NULL) {
      if (strstr(dir2->d_name,"cdf")) {
	run2ls.push_back(dir2->d_name);
      }
    }
  } else {
    std::cout << "Could not open " << dirstring2 << std::endl;
  }
  
  std::sort(run2ls.begin(), run2ls.end());
  run2ls.erase(run2ls.begin(),run2ls.begin()+18);
  run2ls.erase(run2ls.begin()+35,run2ls.begin()+58);
  
  std::cout << "run2ls size: " << run2ls.size() << std::endl;
  std::cout << "First file: " << run2ls[0] << std::endl;
  std::cout << "Last file: " << run2ls[run2ls.size()-1] << std::endl;
  ///////////////////////////////////////////////////////////////    
  
  ///////////////////////////////////////////////////////////////    
  // loop over timesteps
  for(int loopnum = 0; loopnum<run1ls.size(); loopnum++ ){

    filename1 = dirstring1;
    filename1.append("/");
    filename1.append(run1ls[loopnum]);
    std::cout << filename1 << std::endl;

    filename2 = dirstring2;
    filename2.append("/");
    filename2.append(run2ls[loopnum]);
    std::cout << filename2 << std::endl;
    
    long status1 = kameleon1.open(filename1);
    std::cout << "Opened: " << filename1 << " with status: " << status1 << std::endl;
    std::cout << "ccmc::FileReader::OK = " << ccmc::FileReader::OK << std::endl;
    
    if(kameleon1.doesVariableExist("bz")){kameleon1.loadVariable("bz");}else{return 1;}
    if(kameleon1.doesVariableExist("jx")){kameleon1.loadVariable("jx");}else{return 1;}
    if(kameleon1.doesVariableExist("rho")){kameleon1.loadVariable("rho");}else{return 1;}
    if(kameleon1.doesVariableExist("ux")){kameleon1.loadVariable("ux");}else{return 1;}
    
    long status2 = kameleon2.open(filename2);
    std::cout << "Opened file: " << filename2 << " with status: " << status2 << std::endl;
    std::cout << "FileReader::OK = " << ccmc::FileReader::OK << std::endl;
    
    if(kameleon2.doesVariableExist("bz")){kameleon2.loadVariable("bz");}else{return 1;}
    if(kameleon2.doesVariableExist("jx")){kameleon2.loadVariable("jx");}else{return 1;}
    if(kameleon2.doesVariableExist("rho")){kameleon2.loadVariable("rho");}else{return 1;}
    if(kameleon2.doesVariableExist("ux")){kameleon2.loadVariable("ux");}else{return 1;}
    
    // INTERPOLATION
    ccmc::Interpolator * interpolator1 = kameleon1.createNewInterpolator();
    ccmc::Interpolator * interpolator2 = kameleon2.createNewInterpolator();
    boost::numeric::ublas::matrix<float> value1(4,npoinx*npoiny*npoinz);
    boost::numeric::ublas::matrix<float> value2(4,npoinx*npoiny*npoinz);
    boost::numeric::ublas::matrix<float> diff(4,npoinx*npoiny*npoinz);
    
    std::cout << "Starting Interpolations and Differencing" << std::endl;
    int l = 0;
    for (int k = 0; k < npoinz; k++){
      for (int j = 0; j < npoiny; j++){
	for (int i = 0; i < npoinx; i++){
	  value1(0,l) = interpolator1->interpolate("bz", xcord[i], ycord[j], zcord[k]);
	  value1(1,l) = interpolator1->interpolate("jx", xcord[i], ycord[j], zcord[k]);
	  value1(2,l) = interpolator1->interpolate("rho", xcord[i], ycord[j], zcord[k]);
	  value1(3,l) = interpolator1->interpolate("ux", xcord[i], ycord[j], zcord[k]);
	  value2(0,l) = interpolator2->interpolate("bz", xcord[i], ycord[j], zcord[k]);
	  value2(1,l) = interpolator2->interpolate("jx", xcord[i], ycord[j], zcord[k]);
	  value2(2,l) = interpolator2->interpolate("rho", xcord[i], ycord[j], zcord[k]);
	  value2(3,l) = interpolator2->interpolate("ux", xcord[i], ycord[j], zcord[k]);
	  diff(0,l) = (( value1(0,l)-value2(0,l) ) / ( (value1(0,l)+value2(0,l)) / 2.0 ))*100.0;
	  diff(1,l) = (( value1(1,l)-value2(1,l) ) / ( (value1(1,l)+value2(1,l)) / 2.0 ))*100.0;
	  diff(2,l) = (( value1(2,l)-value2(2,l) ) / ( (value1(2,l)+value2(2,l)) / 2.0 ))*100.0;
	  diff(3,l) = (( value1(3,l)-value2(3,l) ) / ( (value1(3,l)+value2(3,l)) / 2.0 ))*100.0;
	  l++;
	}
      }
    }
    std::cout << "Interpolations and Differencing Complete" << std::endl;
    ///////////////////////////////////////////////////////////////    
    
    ///////////////////////////////////////////////////////////////
    std::cout << "Starting VTK Write" << std::endl;
    std::string resultfilename;
    resultfilename = dirname;
    resultfilename.append("Precondition/");
    resultfilename.append(dir1s);
    resultfilename.append("_vs_");
    resultfilename.append(dir2s);
    std::string com;
    com = "mkdir -p ";
    com.append(resultfilename); 
    system(com.c_str());
    
    resultfilename.append("/pcdiff");
    resultfilename.append(boost::lexical_cast<std::string>(loopnum));
    resultfilename.append(".vtk");
    
    vtk(resultfilename,xcord,ycord,zcord,diff);
    std::cout << "Wrote: " << resultfilename << std::endl;
    ///////////////////////////////////////////////////////////////
    
    kameleon1.close();
    kameleon2.close();
    value1.clear();
    value2.clear();
    diff.clear();
    
  }

  return 0;
  
}
