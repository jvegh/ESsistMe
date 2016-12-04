#include "VariableMetricBuilder.h"
#include "GradientCalculator.h"
#include "MinimumState.h"
#include "MinimumError.h"
#include "FunctionGradient.h"
#include "FunctionMinimum.h"
#include "MnLineSearch.h"
#include "MinimumSeed.h"
#include "MnFcn.h"
#include "MnMachinePrecision.h"
#include "MnPosDef.h"
#include "MnParabolaPoint.h"
#include "LaSum.h"
#include "LaProd.h"
#include "MnStrategy.h"
#include "MnHesse.h"

double inner_product(const LAVector&, const LAVector&);

FunctionMinimum VariableMetricBuilder::minimum(const MnFcn& fcn, const GradientCalculator& gc, const MinimumSeed& seed, const MnStrategy& strategy, unsigned int maxfcn, double edmval) const {

  FunctionMinimum min = minimum(fcn, gc, seed, maxfcn, edmval);

  if( (strategy.strategy() == 2) || 
      (strategy.strategy() == 1 && min.error().dcovar() > 0.05) ) {
//  std::cout<<"MnMigrad will verify convergence and error matrix."<<std::endl;
    MinimumState st = MnHesse(strategy)(fcn, min.state(), min.seed().trafo());
    min.add(st);
  }
  if(!min.isValid()) std::cout<<"FunctionMinimum is invalid."<<std::endl;

  return min;
}

FunctionMinimum VariableMetricBuilder::minimum(const MnFcn& fcn, const GradientCalculator& gc, const MinimumSeed& seed, unsigned int maxfcn, double edmval) const {

  edmval *= 0.0001;

//   std::cout<<"VariableMetricBuilder convergence when edm < "<<edmval<<std::endl;
  if(seed.parameters().vec().size() == 0) {
    return FunctionMinimum(seed, fcn.up());
  }

  const MnMachinePrecision& prec = seed.precision();

  std::vector<MinimumState> result;
//   result.reserve(1);
  result.reserve(8);

//   double edm = estimator().estimate(seed.gradient(), seed.error());
  double edm = seed.state().edm();
  if(edm < 0.) {
    std::cout<<"VariableMetricBuilder: initial matrix not pos.def."<<std::endl;
    assert(!seed.error().isPosDef());
    return FunctionMinimum(seed, fcn.up());
  }

//   result.push_back(MinimumState(seed.parameters(), seed.error(), seed.gradient(), edm, fcn.numOfCalls()));
  result.push_back(seed.state());

  // iterate until edm is small enough or max # of iterations reached
  edm *= (1. + 3.*seed.error().dcovar());
  MnLineSearch lsearch;
  MnAlgebraicVector step(seed.gradient().vec().size());
  int iterno=0;	// Count iterations
  do {   
    
//     const MinimumState& s0 = result.back();
    MinimumState s0 = result.back();
    
    step = -1.*s0.error().invHessian()*s0.gradient().vec();

    double gdel = inner_product(step, s0.gradient().grad());
    if(gdel > 0.) {
      std::cout<<"VariableMetricBuilder: matrix not pos.def."<<std::endl;
      std::cout<<"gdel > 0: "<<gdel<<std::endl;
      MnPosDef psdf;
      s0 = psdf(s0, prec);
      step = -1.*s0.error().invHessian()*s0.gradient().vec();
      gdel = inner_product(step, s0.gradient().grad());
      std::cout<<"gdel: "<<gdel<<std::endl;
      if(gdel > 0.) {
	result.push_back(s0);
	return FunctionMinimum(seed, result, fcn.up());
      }
    }
    MnParabolaPoint pp = lsearch(fcn, s0.parameters(), step, gdel, prec);
    if(fabs(pp.y() - s0.fval()) < prec.eps()) {
      std::cout<<"VariableMetricBuilder: no improvement"<<std::endl;
      break; //no improvement
    }
    MinimumParameters p(s0.vec() + pp.x()*step, pp.y());
    FunctionGradient g = gc(p, s0.gradient());

    edm = estimator().estimate(g, s0.error());
    if(edm < 0.) {
      std::cout<<"VariableMetricBuilder: matrix not pos.def."<<std::endl;
      std::cout<<"edm < 0"<<std::endl;
      MnPosDef psdf;
      s0 = psdf(s0, prec);
      edm = estimator().estimate(g, s0.error());
      if(edm < 0.) {
	result.push_back(s0);
	return FunctionMinimum(seed, result, fcn.up());
      }
    } 
    MinimumError e = errorUpdator().update(s0, p, g);
    result.push_back(MinimumState(p, e, g, edm, fcn.numOfCalls())); 
//     result[0] = MinimumState(p, e, g, edm, fcn.numOfCalls()); 
    edm *= (1. + 3.*e.dcovar());
	iterno++;
	std::cout<<"iter#"<<iterno<<", evals:"<<fcn.numOfCalls()<<", chisq:"<<s0.fval()<<std::endl;
  } while(edm > edmval && fcn.numOfCalls() < maxfcn);
  
  if(fcn.numOfCalls() >= maxfcn) {
    std::cout<<"VariableMetricBuilder: call limit exceeded."<<std::endl;
    return FunctionMinimum(seed, result, fcn.up(), FunctionMinimum::MnReachedCallLimit());
  }

  if(edm > edmval) {
    if(edm < fabs(prec.eps2()*result.back().fval())) {
      std::cout<<"VariableMetricBuilder: machine accuracy limits further improvement."<<std::endl;
      return FunctionMinimum(seed, result, fcn.up());
    } else if(edm < 10.*edmval) {
      return FunctionMinimum(seed, result, fcn.up());
    } else {
      std::cout<<"VariableMetricBuilder: finishes without convergence."<<std::endl;
      std::cout<<"VariableMetricBuilder: edm= "<<edm<<" requested: "<<edmval<<std::endl;
      return FunctionMinimum(seed, result, fcn.up(), FunctionMinimum::MnAboveMaxEdm());
    }
  }
//   std::cout<<"result.back().error().dcovar()= "<<result.back().error().dcovar()<<std::endl;

  return FunctionMinimum(seed, result, fcn.up());
}
