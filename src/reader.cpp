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
  std::string filename1, filename2, filename1o;
  std::string variable;
  std::vector<int> myvars;
  bool firstrun=true, CALCDIFF=false;
  int i,j,k,m;

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
  for (runi = 0; runi < chooserun.size(); runi++){
    if (runi==3 || runi==7 || runi==11 || runi==15){ // SKIP LFM RUNS
      std::cout << "Skipping Run " << runi << std::endl;
      continue;
    }
    std::cout << "File " << runi << ": " << chooserun[runi] << std::endl;
    dset1 = runi;

    // WOULD NEED TO CREATE A LOOP THROUGH ALL FILES IN BOTH DATA SETS
    // GET USER TO PROVIDE DIRECTORIES OF TWO MODEL OUTPUTS
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
	if(strcmp(dir->d_name,"..") && strcmp(dir->d_name,".")){
	  i++;
	  run1ls.push_back(dir->d_name);
	}
      }
    }

    // SORT THE STRING SO FILENAMES ARE IN ORDER
    std::sort(run1ls.begin(), run1ls.end());

    int loopnum;
    for(loopnum=0; loopnum<run1ls.size(); loopnum++ ){
	filename1 = dirstring;
	filename1.append("/");
	filename1.append(run1ls[loopnum]);
	filename1o = filename1;
	std::string com;
	std::string comx;

	int wasgz = 0;

	if (filename1.substr(filename1.find_last_of(".")) == ".gz") {
	  filename1o = filename1o.erase(filename1.size() - 3); // Remove .gz extension

	  bool b = std::ifstream(filename1o.c_str()).good(); // If unzipped file exists
	  if (b) {
	    continue;
	  } else {
	    wasgz = 1; // If no unzipped file, unzip it and then delete unzipped file when done.
	    com = "gunzip -c " + filename1 + " > " + filename1o;
	    comx = "rm -f " + filename1o;
	    std::cout << "Unzipping " << filename1 << std::endl;
	    system(com.c_str());
	    filename1 = filename1o;
	  }
	}
	
	std::cout << com << std::endl;
	std::cout << "Reading " << filename1 << std::endl;
	long status = kameleon1.open(filename1);
	std::cout << "Opened " << filename1 << " with status: " << status << std::endl;
	std::cout << "ccmc::FileReader::OK = " << ccmc::FileReader::OK << std::endl;

	if(kameleon1.doesVariableExist("bz")){kameleon1.loadVariable("bz");}else{return 1;}
	if(kameleon1.doesVariableExist("jx")){kameleon1.loadVariable("jx");}else{return 1;}
	if(kameleon1.doesVariableExist("rho")){kameleon1.loadVariable("rho");}else{return 1;}
	if(kameleon1.doesVariableExist("ux")){kameleon1.loadVariable("ux");}else{return 1;}

	// INTERPOLATION #1
	ccmc::Interpolator * interpolator = kameleon1.createNewInterpolator();
	std::cout << "Starting Interpolation" << std::endl;
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
	
	std::cout << "Interpolation Done\nUnloading Variables" << std::endl;
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

	float val, val1, val2;
	FILE *myfile;
	myfile = fopen(resultfilename.c_str(),"w");
	fprintf(myfile, "# vtk DataFile Version 3.0\n");
	fprintf(myfile, "Brian's Data\nBINARY\n");
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
	fprintf(myfile, "\nSCALARS Bz FLOAT 1\n");
	fprintf(myfile, "LOOKUP_TABLE default\n");
	for (i=0; i<npoinx*npoiny*npoinz; i++){
	  //val = FloatSwap(value[m][i]);
	  val = FloatSwap(value(0,i));
	  fwrite((void *)&val, sizeof(float), 1, myfile);
	}
	fprintf(myfile, "\nSCALARS Jx FLOAT 1\n");
	fprintf(myfile, "LOOKUP_TABLE default\n");
	for (i=0; i<npoinx*npoiny*npoinz; i++){
	  //val = FloatSwap(value[m][i]);
	  val = FloatSwap(value(1,i));
	  fwrite((void *)&val, sizeof(float), 1, myfile);
	}
	fprintf(myfile, "\nSCALARS rho FLOAT 1\n");
	fprintf(myfile, "LOOKUP_TABLE default\n");
	for (i=0; i<npoinx*npoiny*npoinz; i++){
	  //val = FloatSwap(value[m][i]);
	  val = FloatSwap(value(2,i));
	  fwrite((void *)&val, sizeof(float), 1, myfile);
	}
	fprintf(myfile, "\nSCALARS Ux FLOAT 1\n");
	fprintf(myfile, "LOOKUP_TABLE default\n");
	for (i=0; i<npoinx*npoiny*npoinz; i++){
	  //val = FloatSwap(value[m][i]);
	  val = FloatSwap(value(3,i));
	  fwrite((void *)&val, sizeof(float), 1, myfile);
	}
	fclose(myfile);
	std::cout << "Wrote: " << resultfilename << std::endl;
	kameleon1.close();
	value.clear();

	if (wasgz == 1) {
	  std::cout << "Removing " << filename1o << std::endl;
	  system(comx.c_str());
	}

    }
  }
  //delete interpolator;
  return 0;
}
