#include <boost/numeric/ublas/matrix.hpp>

namespace bnu = boost::numeric::ublas;
bnu::matrix<double> demo() {
  return bnu::identity_matrix<double>(3);
}
int main () {
  bnu::matrix<double> m = demo();
  std::cout << m(0,0) << "\n";
}
