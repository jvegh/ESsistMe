#ifndef MN_MnEigen_H_
#define MN_MnEigen_H_

class MnUserCovariance;

/**
   API class for calculating the eigenvalues of symmetric matrix
 */

#include <vector>

class MnEigen {

public:

  MnEigen() {}

  ~MnEigen() {}

  /// calculate the eigenvalues
  std::vector<double> operator()(const MnUserCovariance&) const;

private:

};

#endif //MN_MnEigen_H_
