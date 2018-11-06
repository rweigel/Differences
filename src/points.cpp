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

#include <fstream>
#include <sstream>
#include <iostream>

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

namespace bnu = boost::numeric::ublas;

int main (int argc, char * argv[]){

  std::string dir1string;
  std::string filename1input;
  std::string filename1output;
  std::string tmpstr;

  const char* dir1input = argv[1];

  DIR *d1;
  struct dirent *dir1struct;
  std::string filename1str;
  std::vector<std::string> run1ls;

  ccmc::Kameleon kameleon1;

  bnu::matrix<double> xyz = points(argv[2]);
  int Npts = xyz.size1();
  std::cout << "N = " << Npts << "\n";
  boost::numeric::ublas::matrix<double> value1(14,Npts);

  std::cout << xyz(0,0) << "\n";

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
    if(kameleon1.doesVariableExist("jx")){kameleon1.loadVariable("jx");}else{return 1;}
    if(kameleon1.doesVariableExist("jy")){kameleon1.loadVariable("jy");}else{return 1;}
    if(kameleon1.doesVariableExist("jz")){kameleon1.loadVariable("jz");}else{return 1;}
    if(kameleon1.doesVariableExist("ux")){kameleon1.loadVariable("ux");}else{return 1;}
    if(kameleon1.doesVariableExist("uy")){kameleon1.loadVariable("uy");}else{return 1;}
    if(kameleon1.doesVariableExist("uz")){kameleon1.loadVariable("uz");}else{return 1;}
    if(kameleon1.doesVariableExist("p")){kameleon1.loadVariable("p");}else{return 1;}
    if(kameleon1.doesVariableExist("rho")){kameleon1.loadVariable("rho");}else{return 1;}
    std::cout << filename1input << ": Loading variables finished." << std::endl;

    ccmc::Interpolator * interpolator1 = kameleon1.createNewInterpolator();

    std::cout << filename1input << ": Interpolation started." << std::endl;

    for (int k = 0; k < Npts; k++)
      {

	value1(0,k) = interpolator1->interpolate("x",   xyz(0,k), xyz(1,k), xyz(2,k));
	value1(1,k) = interpolator1->interpolate("y",   xyz(0,k), xyz(1,k), xyz(2,k));
	value1(2,k) = interpolator1->interpolate("z",   xyz(0,k), xyz(1,k), xyz(2,k));
	value1(3,k) = interpolator1->interpolate("bx",  xyz(0,k), xyz(1,k), xyz(2,k));
	value1(4,k) = interpolator1->interpolate("by",  xyz(0,k), xyz(1,k), xyz(2,k));
	value1(5,k) = interpolator1->interpolate("bz",  xyz(0,k), xyz(1,k), xyz(2,k));
	value1(6,k) = interpolator1->interpolate("jx",  xyz(0,k), xyz(1,k), xyz(2,k));
	value1(7,k) = interpolator1->interpolate("jy",  xyz(0,k), xyz(1,k), xyz(2,k));
	value1(8,k) = interpolator1->interpolate("jz",  xyz(0,k), xyz(1,k), xyz(2,k));
	value1(9,k) = interpolator1->interpolate("ux",  xyz(0,k), xyz(1,k), xyz(2,k));
	value1(10,k) = interpolator1->interpolate("uy", xyz(0,k), xyz(1,k), xyz(2,k));
	value1(11,k) = interpolator1->interpolate("uz", xyz(0,k), xyz(1,k), xyz(2,k));
	value1(12,k) = interpolator1->interpolate("p",  xyz(0,k), xyz(1,k), xyz(2,k));
	value1(13,k) = interpolator1->interpolate("rho",xyz(0,k), xyz(1,k), xyz(2,k));

    }
    std::cout << filename1input << ": Interpolation finished." << std::endl;
    ///////////////////////////////////////////////////////////////    

    kameleon1.close();

    ///////////////////////////////////////////////////////////////
    filename1output = dir1input;
    boost::replace_all(filename1output, "data/", "output/");
    filename1output.append("/points");
    tmpstr = "mkdir -p ";
    tmpstr.append(filename1output);
    system(tmpstr.c_str());
    filename1output.append("/points_");
    if (loopnum < 10) {
      filename1output.append("0");
    }
    filename1output.append(boost::lexical_cast<std::string>(loopnum));
    filename1output.append(".txt");
    
    std::cout << filename1output << ": Writing started." << std::endl;  
    writepoints(filename1output,value1,xyz);
    std::cout << filename1output << ": Writing finished." << std::endl;  
    ///////////////////////////////////////////////////////////////
    
    value1.clear();

  }
  return 0;
  
}
