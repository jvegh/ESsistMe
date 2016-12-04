#include "MnEigen.h"
#include "MnUserCovariance.h"
#include "MnMatrix.h"

LAVector eigenvalues(const LASymMatrix&);

std::vector<double> MnEigen::operator()(const MnUserCovariance& covar) const {

  LASymMatrix cov(covar.nrow());
  for(unsigned int i = 0; i < covar.nrow(); i++)
    for(unsigned int j = i; j < covar.nrow(); j++)
      cov(i,j) = covar(i,j);

  LAVector eigen = eigenvalues(cov);

  std::vector<double> result(eigen.data(), eigen.data()+covar.nrow());
  return result;
}
