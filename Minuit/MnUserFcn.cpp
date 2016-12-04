#include "MnUserFcn.h"
#include "FCNBase.h"
#include "MnUserTransformation.h"

double MnUserFcn::operator()(const MnAlgebraicVector& v) const {

  theNumCall++;
  return fcn()( theTransform(v) );
}
