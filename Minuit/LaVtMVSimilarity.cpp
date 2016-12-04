#include <cstring>
#include "LASymMatrix.h"
#include "LAVector.h"
#include "LaProd.h"

double mnddot(unsigned int, const double*, int, const double*, int);

double similarity(const LAVector& avec, const LASymMatrix& mat) {

  LAVector tmp = mat*avec;

  double value = mnddot(avec.size(), avec.data(), 1, tmp.data(), 1);
  return value;
}
