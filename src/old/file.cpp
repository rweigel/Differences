#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <boost/numeric/ublas/matrix.hpp>
#include "grid.h"
#include <boost/numeric/ublas/io.hpp>

namespace bnu = boost::numeric::ublas;

int main (int argc, char * argv[])
{
  int Npts = atoi(argv[4]);
  std::cout << "Number of lines to read = " << Npts << "\n";
  bnu::matrix<double> xyz = points(argv[3]);
  return 0;
}

