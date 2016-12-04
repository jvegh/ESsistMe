#include "MnApplication.h"
#include "FunctionMinimum.h"
#include "ModularFunctionMinimizer.h"

FunctionMinimum MnApplication::operator()(unsigned int maxfcn, double toler) {

  assert(theState.isValid());
  unsigned int npar = variableParameters();
//   assert(npar > 0);
  if(maxfcn == 0) maxfcn = 200 + 100*npar + 5*npar*npar;
  FunctionMinimum min = minimizer().minimize(theFCN, theState, theStrategy, maxfcn, toler);
  theNumCall += min.nfcn();
  theState = min.userState();
  return min;
}

// facade: forward interface of MnUserParameters and MnUserTransformation
// via MnUserParameterState

//access to parameters (row-wise)
const std::vector<MinuitParameter>& MnApplication::minuitParameters() const {
  return theState.minuitParameters();
}
//access to parameters and errors in column-wise representation 
std::vector<double> MnApplication::params() const {return theState.params();}
std::vector<double> MnApplication::errors() const {return theState.errors();}

//access to single parameter
const MinuitParameter& MnApplication::parameter(unsigned int i) const {
  return theState.parameter(i);
}

//add free parameter
void MnApplication::add(const char* name, double val, double err) {
  theState.add(name, val, err);
}
//add limited parameter
void MnApplication::add(const char* name, double val, double err, double low, double up) {
  theState.add(name, val, err, low, up);
}
//add const parameter
void MnApplication::add(const char* name, double val) {
  theState.add(name, val);
}

//interaction via external number of parameter
void MnApplication::fix(unsigned int i) {theState.fix(i);}
void MnApplication::release(unsigned int i) {theState.release(i);}
void MnApplication::setValue(unsigned int i, double val) {
  theState.setValue(i, val);
}
void MnApplication::setError(unsigned int i, double val) {
  theState.setError(i, val);
}
void MnApplication::setLimits(unsigned int i, double low, double up) {
  theState.setLimits(i, low, up);
}
void MnApplication::removeLimits(unsigned int i) {theState.removeLimits(i);}

double MnApplication::value(unsigned int i) const {return theState.value(i);}
double MnApplication::error(unsigned int i) const {return theState.error(i);}
  
//interaction via name of parameter
void MnApplication::fix(const char* i) {theState.fix(i);}
void MnApplication::release(const char* i) {theState.release(i);}
void MnApplication::setValue(const char* i, double val) {theState.setValue(i, val);}
void MnApplication::setError(const char* i, double val) {theState.setError(i, val);}
void MnApplication::setLimits(const char* i, double low, double up) {
  theState.setLimits(i, low, up);
}
void MnApplication::removeLimits(const char* i) {theState.removeLimits(i);}
void MnApplication::setPrecision(double eps) {theState.setPrecision(eps);}

double MnApplication::value(const char* i) const {return theState.value(i);}
double MnApplication::error(const char* i) const {return theState.error(i);}
  
//convert name into external number of parameter
unsigned int MnApplication::index(const char* name) const {
  return theState.index(name);
}
//convert external number into name of parameter
const char* MnApplication::name(unsigned int i) const {
  return theState.name(i);
}

// transformation internal <-> external
double MnApplication::int2ext(unsigned int i, double val) const {
  return theState.int2ext(i, val);
}
double MnApplication::ext2int(unsigned int e, double val) const {
  return theState.ext2int(e, val);
}
unsigned int MnApplication::intOfExt(unsigned int ext) const {
  return theState.intOfExt(ext);
}
unsigned int MnApplication::extOfInt(unsigned int internal) const { 
    return theState.extOfInt(internal);
}
unsigned int MnApplication::variableParameters() const {
  return theState.variableParameters();
}

/// The default monitor function simply types out it counts and function value
/// when the requested number of iterations finished
class	FCNMonitor : public FCNMonitorBase
{
  virtual void operator()(const int CurrentIter, ///< Current iteration count
							const int IterFreq,  ///< How frequently to react
							const int NoOfCalls,	///< Number of function calls made
							const double fval,	 ///< The present minimum function value
							const MinimumState &s0)	const ///< The reached minimum state
  {
	  if(IterFreq<=0) return;	// Monitoring disabled 
	  if(CurrentIter%IterFreq) return;	//Monitorize only every IterFreq-th
      std::cout<<std::dec<<"iter#"<<CurrentIter<<", evals:"<<NoOfCalls<<", chisq:"<<fval<<std::endl;
  }
}FCNMonitorDefault;
