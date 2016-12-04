#include "SimplexBuilder.h"
#include "FunctionMinimum.h"
#include "MnFcn.h"
#include "MinimumSeed.h"
#include "SimplexParameters.h"
#include "MinimumState.h"

FunctionMinimum SimplexBuilder::minimum(const MnFcn& mfcn, const GradientCalculator&, const MinimumSeed& seed, const MnStrategy&, unsigned int maxfcn, double minedm) const {
  
  const MnMachinePrecision& prec = seed.precision();
  MnAlgebraicVector x = seed.parameters().vec();
  MnAlgebraicVector step = 10.*seed.gradient().gstep();
  
  unsigned int n = x.size();
  double wg = 1./double(n);
  double alpha = 1., beta = 0.5, gamma = 2., rhomin = 4., rhomax = 8.;
  double rho1 = 1. + alpha;
//   double rho2 = rho1 + alpha*gamma;
//change proposed by david sachs (fnal)
  double rho2 = 1. + alpha*gamma;


  std::vector<std::pair<double, MnAlgebraicVector> > simpl; simpl.reserve(n+1);
  simpl.push_back(std::pair<double, MnAlgebraicVector>(seed.fval(), x));

  unsigned int jl = 0, jh = 0;
  double amin = seed.fval(), aming = seed.fval();

  for(unsigned int i = 0; i < n; i++) {
    double dmin = 8.*prec.eps2()*(fabs(x(i)) + prec.eps2());
    if(step(i) < dmin) step(i) = dmin;
    x(i) += step(i);
    double tmp = mfcn(x);
    if(tmp < amin) {
      amin = tmp;
      jl = i+1;
    }
    if(tmp > aming) {
      aming = tmp;
      jh = i+1;
    }
    simpl.push_back(std::pair<double, MnAlgebraicVector>(tmp, x));
    x(i) -= step(i);
  }
  SimplexParameters simplex(simpl, jh, jl);
  
  do {
    amin = simplex(jl).first;
    jl = simplex.jl();
    jh = simplex.jh();
    MnAlgebraicVector pbar(n);
    for(unsigned int i = 0; i < n+1; i++) {
      if(i == jh) continue;
      pbar += (wg*simplex(i).second);
    }

    MnAlgebraicVector pstar = (1. + alpha)*pbar - alpha*simplex(jh).second;
    double ystar = mfcn(pstar);
    
    if(ystar > amin) {
      if(ystar < simplex(jh).first) {
	simplex.update(ystar, pstar);
	if(jh != simplex.jh()) continue;
      } 
      MnAlgebraicVector pstst = beta*simplex(jh).second + (1. - beta)*pbar;
      double ystst = mfcn(pstst);
      if(ystst > simplex(jh).first) break; 
      simplex.update(ystst, pstst);
      continue;
    }
    
    MnAlgebraicVector pstst = gamma*pstar + (1. - gamma)*pbar;
    double ystst = mfcn(pstst);
    
    double y1 = (ystar - simplex(jh).first)*rho2;
    double y2 = (ystst - simplex(jh).first)*rho1;
    double rho = 0.5*(rho2*y1 - rho1*y2)/(y1 - y2);
    if(rho < rhomin) {
      if(ystst < simplex(jl).first) simplex.update(ystst, pstst);
      else simplex.update(ystar, pstar);
      continue;
    }
    if(rho > rhomax) rho = rhomax;
    MnAlgebraicVector prho = rho*pbar + (1. - rho)*simplex(jh).second;
    double yrho = mfcn(prho);
    if(yrho < simplex(jl).first && yrho < ystst) {
      simplex.update(yrho, prho);
      continue;
    }
    if(ystst < simplex(jl).first) {
      simplex.update(ystst, pstst);
      continue;
    }
    if(yrho > simplex(jl).first) {
      if(ystst < simplex(jl).first) simplex.update(ystst, pstst);
      else simplex.update(ystar, pstar);
      continue;
    }
    if(ystar > simplex(jh).first) {
      pstst = beta*simplex(jh).second + (1. - beta)*pbar;
      ystst = mfcn(pstst);
      if(ystst > simplex(jh).first) break; 
      simplex.update(ystst, pstst);
    }
  } while(simplex.edm() > minedm && mfcn.numOfCalls() < maxfcn);

  amin = simplex(jl).first;
  jl = simplex.jl();
  jh = simplex.jh();

  MnAlgebraicVector pbar(n);
  for(unsigned int i = 0; i < n+1; i++) {
    if(i == jh) continue;
    pbar += (wg*simplex(i).second);
  }
  double ybar = mfcn(pbar);
  if(ybar < amin) simplex.update(ybar, pbar);
  else {
    pbar = simplex(jl).second;
    ybar = simplex(jl).first;
  }

  MnAlgebraicVector dirin = simplex.dirin();
//   scale to sigmas on parameters werr^2 = dirin^2 * (up/edm) 
  dirin *= sqrt(mfcn.up()/simplex.edm());

  MinimumState st(MinimumParameters(pbar, dirin, ybar), simplex.edm(), mfcn.numOfCalls());

  if(mfcn.numOfCalls() > maxfcn) {
    std::cout<<"Simplex did not converge, #fcn calls exhausted."<<std::endl;
    return FunctionMinimum(seed, std::vector<MinimumState>(1, st), mfcn.up(), FunctionMinimum::MnReachedCallLimit());
  }
  if(simplex.edm() > minedm) {
    std::cout<<"Simplex did not converge, edm > minedm."<<std::endl;
    return FunctionMinimum(seed, std::vector<MinimumState>(1, st), mfcn.up(), FunctionMinimum::MnAboveMaxEdm());
  }
  
  return FunctionMinimum(seed, std::vector<MinimumState>(1, st), mfcn.up());
}
