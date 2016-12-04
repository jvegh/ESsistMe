#ifndef MN_MnLineSearch_H_
#define MN_MnLineSearch_H_

#include "MnMatrix.h"

class MnFcn;
class MinimumParameters;
class MnMachinePrecision;
class MnParabolaPoint;

class MnLineSearch  {

public:

  MnLineSearch() {}

  ~MnLineSearch() {}

  MnParabolaPoint operator()(const MnFcn&, const MinimumParameters&, const MnAlgebraicVector&, double, const MnMachinePrecision&) const;

private:

};
#endif //MN_MnLineSearch_H_
