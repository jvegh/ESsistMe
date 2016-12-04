#include "NegativeG2LineSearch.h"
#include "MnFcn.h"
#include "MinimumState.h"
#include "GradientCalculator.h"
#include "MnMachinePrecision.h"
#include "MnLineSearch.h"
#include "MnParabolaPoint.h"
#include "VariableMetricEDMEstimator.h"

MinimumState NegativeG2LineSearch::operator()(const MnFcn& fcn, const MinimumState& st, const  GradientCalculator& gc, const MnMachinePrecision& prec) const {

  bool negG2 = hasNegativeG2(st.gradient(), prec);
  if(!negG2) return st;

  unsigned int n = st.parameters().vec().size();
  FunctionGradient dgrad = st.gradient();
  MinimumParameters pa = st.parameters();
  bool iterate = false;
  unsigned int iter = 0;
  do {
    iterate = false;
    for(unsigned int i = 0; i < n; i++) {
      if(dgrad.g2()(i) < prec.eps2()) {
	// do line search if second derivative negative
	MnAlgebraicVector step(n);
	MnLineSearch lsearch;
 	step(i) = dgrad.gstep()(i)*dgrad.vec()(i);
	if(fabs(dgrad.vec()(i)) >  prec.eps2()) 
	  step(i) *= (-1./fabs(dgrad.vec()(i)));
	double gdel = step(i)*dgrad.vec()(i);
	MnParabolaPoint pp = lsearch(fcn, pa, step, gdel, prec);
	step *= pp.x();
	pa = MinimumParameters(pa.vec() + step, pp.y());    
	dgrad = gc(pa, dgrad);         
	iterate = true;
	break;
      } 
    }
  } while(iter++ < 2*n && iterate);

  MnAlgebraicSymMatrix mat(n);
  for(unsigned int i = 0; i < n; i++)	
    mat(i,i) = (fabs(dgrad.g2()(i)) > prec.eps2() ? 1./dgrad.g2()(i) : 1.);

  MinimumError err(mat, 1.);
  double edm = VariableMetricEDMEstimator().estimate(dgrad, err);

  return MinimumState(pa, err, dgrad, edm, fcn.numOfCalls());
}

bool NegativeG2LineSearch::hasNegativeG2(const FunctionGradient& grad, const MnMachinePrecision& prec) const {
  
  for(unsigned int i = 0; i < grad.vec().size(); i++) 
    if(grad.g2()(i) < prec.eps2()) return true;

  return false;
}
