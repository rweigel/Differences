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

  ccmc::Kameleon kameleon1, kameleon2;
  std::string filename1, filename2;
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

  DIR *d;
  struct dirent *dir;

  int dset1 [24] = {0,0,0,1,1,2,4,4,4,5,5,6,8,8,8,9,9,10,12,12,12,13,13,14};
  int dset2 [24] = {1,2,3,2,3,3,5,6,7,6,7,7,9,10,11,10,11,11,13,14,15,14,15,15};
  int runi;
  for ( runi = 0; runi < 24; runi++){
    if(dset2[runi]==3 || dset2[runi]==7 || dset2[runi]==11 || dset2[runi]==15){
      std::cout << "Skipping Run " << runi << std::endl;
      continue;
    }
    std::cout << "Dataset 1" << ": " << chooserun[dset1[runi]] << std::endl;
    std::cout << "Dataset 2" << ": " << chooserun[dset2[runi]] << std::endl;
    
    std::vector<std::string> run1ls;
    std::string dirstring;
    dirstring = dirname;
    dirstring.append(chooserun[dset1[runi]]);
    dirstring.append("/GM_CDF");
    d = opendir(dirstring.c_str());
    if (d) {
      while ((dir = readdir(d)) != NULL) {
	if(strcmp(dir->d_name,"..") && strcmp(dir->d_name,".")){
	  //i++;
	  run1ls.push_back(dir->d_name);
	}
      }
    }
    // SORT THE STRING SO FILENAMES ARE IN ORDER
    std::sort(run1ls.begin(), run1ls.end());
    
    std::vector<std::string> run2ls;
    std::string dirstring3;
    std::cout << "SETTING DIRSTRING3" << std::endl;
    dirstring3 = dirname;
    dirstring3.append(chooserun[dset2[runi]]);
    dirstring3.append("/GM_CDF");
    d = opendir(dirstring3.c_str());
    if (d) {
      while ((dir = readdir(d)) != NULL) {
	if(strcmp(dir->d_name,"..") && strcmp(dir->d_name,".")){
	  //i++;
	  run2ls.push_back(dir->d_name);
	}
      }
    }
    // SORT THE STRING SO FILENAMES ARE IN ORDER
    std::sort(run2ls.begin(), run2ls.end());
    //return 0;
    
    int loopnum;
    for(loopnum=0; loopnum<run1ls.size(); loopnum++ ){
      filename1 = dirstring;
      filename1.append("/");
      filename1.append(run1ls[loopnum]);
      std::cout << filename1 << std::endl;
      filename2 = dirstring3;
      filename2.append("/");
      filename2.append(run2ls[loopnum]);
      std::cout << filename2 << std::endl;
      
      long status = kameleon1.open(filename1);
      std::cout << "Opened file: " << filename1 << " with status: " << status << std::endl;
      std::cout << "FileReader::OK = " << ccmc::FileReader::OK << std::endl;
      
      //int numvars = kameleon1.getNumberOfVariables();
      //for(int bb=0; bb<numvars; bb++){
      //	std::cout << "K1 Variable " << bb << "= " << kameleon1.getVariableName(bb) << std::endl;
      //}
      
      if(kameleon1.doesVariableExist("bz")){kameleon1.loadVariable("bz");}else{return 1;}
      if(kameleon1.doesVariableExist("jx")){kameleon1.loadVariable("jx");}else{return 1;}
      if(kameleon1.doesVariableExist("rho")){kameleon1.loadVariable("rho");}else{return 1;}
      if(kameleon1.doesVariableExist("ux")){kameleon1.loadVariable("ux");}else{return 1;}
      
      long status2 = kameleon2.open(filename2);
      std::cout << "Opened file: " << filename2 << " with status: " << status2 << std::endl;
      std::cout << "FileReader::OK = " << ccmc::FileReader::OK << std::endl;
      
      // TEMP LIST ALL VARIABLE NAMES
      //numvars = kameleon2.getNumberOfVariables();
      //for(int ab=0; ab<numvars; ab++){
      //	std::cout << "K2 Variable " << ab << "= " << kameleon2.getVariableName(ab) << std::endl;
      //}
      
      if(kameleon2.doesVariableExist("bz")){kameleon2.loadVariable("bz");}else{return 1;}
      if(kameleon2.doesVariableExist("jx")){kameleon2.loadVariable("jx");}else{return 1;}
      if(kameleon2.doesVariableExist("rho")){kameleon2.loadVariable("rho");}else{return 1;}
      if(kameleon2.doesVariableExist("ux")){kameleon2.loadVariable("ux");}else{return 1;}
      
      // INTERPOLATION #1
      ccmc::Interpolator * interpolator = kameleon1.createNewInterpolator();
      ccmc::Interpolator * interpolator2 = kameleon2.createNewInterpolator();
      boost::numeric::ublas::matrix<float> value(4,npoinx*npoiny*npoinz), \
	value2(4,npoinx*npoiny*npoinz),					\
	diff(4,npoinx*npoiny*npoinz);
      
      std::cout << "Starting Interpolations and Diff" << std::endl;
      int l=0;
      for ( k = 0; k < npoinz; k++){
	for ( j = 0; j < npoiny; j++){
	  for ( i = 0; i < npoinx; i++){
	    value(0,l) = interpolator->interpolate("bz", xcord[i], ycord[j], zcord[k]);
	    value(1,l) = interpolator->interpolate("jx", xcord[i], ycord[j], zcord[k]);
	    value(2,l) = interpolator->interpolate("rho", xcord[i], ycord[j], zcord[k]);
	    value(3,l) = interpolator->interpolate("ux", xcord[i], ycord[j], zcord[k]);
	    value2(0,l) = interpolator2->interpolate("bz", xcord[i], ycord[j], zcord[k]);
	    value2(1,l) = interpolator2->interpolate("jx", xcord[i], ycord[j], zcord[k]);
	    value2(2,l) = interpolator2->interpolate("rho", xcord[i], ycord[j], zcord[k]);
	    value2(3,l) = interpolator2->interpolate("ux", xcord[i], ycord[j], zcord[k]);
	    diff(0,l) = (( value(0,l)-value2(0,l) ) / ( (value(0,l)+value2(0,l)) / 2.0 ))*100.0;
	    diff(1,l) = (( value(1,l)-value2(1,l) ) / ( (value(1,l)+value2(1,l)) / 2.0 ))*100.0;
	    diff(2,l) = (( value(2,l)-value2(2,l) ) / ( (value(2,l)+value2(2,l)) / 2.0 ))*100.0;
	    diff(3,l) = (( value(3,l)-value2(3,l) ) / ( (value(3,l)+value2(3,l)) / 2.0 ))*100.0;
	    l++;
	  }
	}
      }
      
      std::cout << "Interpolations and Diff Complete" << std::endl;
      kameleon1.unloadVariable("bz");
      kameleon1.unloadVariable("jx");
      kameleon1.unloadVariable("rho");
      kameleon1.unloadVariable("ux");
      kameleon2.unloadVariable("bz");
      kameleon2.unloadVariable("jx");
      kameleon2.unloadVariable("rho");
      kameleon2.unloadVariable("ux");
      
      // WRITE THE VTK FILE FOR RENDERING
      std::string resultfilename;
      std::cout << "Starting VTK Write" << std::endl;
      resultfilename = dirname;
      resultfilename.append("Results/");
      resultfilename.append(boost::lexical_cast<std::string>(dset1[runi]));
      resultfilename.append("_");
      resultfilename.append(boost::lexical_cast<std::string>(dset2[runi]));
      resultfilename.append("/Result");
      resultfilename.append(boost::lexical_cast<std::string>(loopnum));
      resultfilename.append(".vtk");
      std::cout << "Result File Name is: " << resultfilename << std::endl;
      
      // BINARY VTK FILE
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
      fprintf(myfile, "\nSCALARS Bz_Diff FLOAT 1\n");
      fprintf(myfile, "LOOKUP_TABLE default\n");
      for (i=0; i<npoinx*npoiny*npoinz; i++){
	//val = FloatSwap(value[m][i]);
	val = FloatSwap(diff(0,i));
	fwrite((void *)&val, sizeof(float), 1, myfile);
      }
      fprintf(myfile, "\nSCALARS Jx_Diff FLOAT 1\n");
      fprintf(myfile, "LOOKUP_TABLE default\n");
      for (i=0; i<npoinx*npoiny*npoinz; i++){
	//val = FloatSwap(value[m][i]);
	val = FloatSwap(diff(1,i));
	fwrite((void *)&val, sizeof(float), 1, myfile);
      }
      fprintf(myfile, "\nSCALARS rho_Diff FLOAT 1\n");
      fprintf(myfile, "LOOKUP_TABLE default\n");
      for (i=0; i<npoinx*npoiny*npoinz; i++){
	//val = FloatSwap(value[m][i]);
	val = FloatSwap(diff(2,i));
	fwrite((void *)&val, sizeof(float), 1, myfile);
      }
      fprintf(myfile, "\nSCALARS Ux_Diff FLOAT 1\n");
      fprintf(myfile, "LOOKUP_TABLE default\n");
      for (i=0; i<npoinx*npoiny*npoinz; i++){
	//val = FloatSwap(value[m][i]);
	val = FloatSwap(diff(3,i));
	fwrite((void *)&val, sizeof(float), 1, myfile);
      }
      fclose(myfile);
      std::cout << "VTK Write Complete" << std::endl;
      // FINISH WRITE OF VTK FILE FOR RENDERING

      kameleon1.close();
      kameleon2.close();
      value.clear();
      value2.clear();
      diff.clear();
      
    }
  }
  return 0;
}
