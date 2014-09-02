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

float FloatSwap( float f )
{
   union
   {
      float f;
      unsigned char b[4];
   } dat1, dat2;

   dat1.f = f;
   dat2.b[0] = dat1.b[3];
   dat2.b[1] = dat1.b[2];
   dat2.b[2] = dat1.b[1];
   dat2.b[3] = dat1.b[0];
   return dat2.f;
}

int main (int argc, char * argv[]){
	ccmc::Kameleon kameleon1, kameleon2;
	std::string filename1, filename2;
	std::string variable;
	std::vector<int> myvars;
	bool firstrun=true, CALCDIFF=false;
	int i,j,k,m;
	
	// CREATING PERSONALIZED MAGNETOSPHERIC GRID
    int npoinx = 345, npoiny = 187, npoinz = 187;
    int xmin=-222, ymin=-47, zmin=ymin;
    int xmax=30, ymax=47, zmax=ymax;
    float *xcord = new float[npoinx];
    xcord[0] = xmin;
    // X COORDINATES
    i = 0;
    while (xcord[i] < xmax){
    	i++;
        if(xcord[i-1] < -30. || xcord[i-1] >= 30.){ xcord[i] = xcord[i-1]+1.; }
        else if(xcord[i-1] < -8. || xcord[i-1] >= 8.){ xcord[i] = xcord[i-1]+0.5; }
        else if(xcord[i-1] <= 0. || xcord[i-1] >= 0.){ xcord[i] = xcord[i-1]+0.25; } 
    }
    i = 0;
    float *ycord = new float[npoiny];
    ycord[0] = ymin;
    // Y COORDINATES
    while (ycord[i] < ymax){
    	i++;
        if(ycord[i-1] < -30. || ycord[i-1] >= 30.){ ycord[i] = ycord[i-1]+1.; }
        else if(ycord[i-1] < -8. || ycord[i-1] >= 8.){ ycord[i] = ycord[i-1]+0.5; }
        else if(ycord[i-1] <= 0. || ycord[i-1] >= 0.){ ycord[i] = ycord[i-1]+0.25; } 
    }
    i = 0;
    float *zcord = new float[npoinz];
    zcord[0] = zmin;
    // Z COORDINATES
    while (zcord[i] < zmax){
    	i++;
        if(zcord[i-1] < -30. || zcord[i-1] >= 30.){ zcord[i] = zcord[i-1]+1.; }
        else if(zcord[i-1] < -8. || zcord[i-1] >= 8.){ zcord[i] = zcord[i-1]+0.5; }
        else if(zcord[i-1] <= 0. || zcord[i-1] >= 0.){ zcord[i] = zcord[i-1]+0.25; } 
    }

	
    // GET THE DIRECTORY CONTENTS AND SORT INTO AN ARRAY
    DIR *d;
    struct dirent *dir;
    std::vector<std::string> chooserun;
    d = opendir("/mnt/Disk2/Precondition");
    i=0;
    if (d) {
		while ((dir = readdir(d)) != NULL) {
			if(strcmp(dir->d_name,"..") && strcmp(dir->d_name,".") && strcmp(dir->d_name,"lost+found") && strcmp(dir->d_name,"Results")){
				i++;
				chooserun.push_back(dir->d_name);
			}
		}
    }
	std::sort(chooserun.begin(), chooserun.end());
	int dset1 [3] = {0,1,2};
	int dset2 [3] = {3,4,5};
	int runi;
	for ( runi = 0; runi < 3; runi++){
		/*if(dset2[runi]==3 || dset2[runi]==7 || dset2[runi]==11 || dset2[runi]==15){
			std::cout << "Skipping Run " << runi << std::endl;
			continue;
		}*/
		std::cout << "runi: " << runi << std::endl;
		std::cout << "Dataset 1" << "= " << chooserun[dset1[runi]] << std::endl;
		std::cout << "runi: " << runi << std::endl;
		std::cout << "Dataset 2" << "= " << chooserun[dset2[runi]] << std::endl;

		std::vector<std::string> run1ls;
		std::string dirstring;
		dirstring = "/mnt/Disk2/Precondition/"; dirstring.append(chooserun[dset1[runi]]); dirstring.append("/GM_CDF");
		d = opendir(dirstring.c_str());
		if (d) {
			while ((dir = readdir(d)) != NULL) {
				if(strcmp(dir->d_name,"..") && strcmp(dir->d_name,".")){
					//i++;
					run1ls.push_back(dir->d_name);
				}
			}
		}
		std::cout << "run1ls size: " << run1ls.size() << std::endl;
		// SORT THE STRING SO FILENAMES ARE IN ORDER
		std::sort(run1ls.begin(), run1ls.end());
		
		std::vector<std::string> run2ls;
		std::string dirstring3;
		std::cout << "SETTING DIRSTRING3" << std::endl;
		dirstring3 = "/mnt/Disk2/Precondition/"; dirstring3.append(chooserun[dset2[runi]]); dirstring3.append("/GM_CDF");
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
			boost::numeric::ublas::matrix<float> value(4,npoinx*npoiny*npoinz), value2(4,npoinx*npoiny*npoinz), diff(4,npoinx*npoiny*npoinz);

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

			// WRITE THE VTK FILE FOR RENDERING
			
			std::string resultfilename;

			std::cout << "Starting VTK Write" << std::endl;
			resultfilename = "/mnt/Disk2/Precondition/Results/";
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
			kameleon1.close();
			kameleon2.close();
			value.clear();
			value2.clear();
			diff.clear();
			
	    }
	}
    return 0;
    
}
