#include "FunctionMinimum.h"
#include "MnPrint.h"
#include "VariableMetricMinimizer.h"
#include "MnMigrad.h"

#include "FCNGradientBase.h"

class Quad3F : public FCNGradientBase {

public:

  Quad3F() : theErrorDef(1.) {}

  ~Quad3F() {}

  double operator()(const std::vector<double>& par) const {

    double x = par[0];
    double y = par[1];
    double z = par[2];

    return ( x*x  + y*y + z*z );
  }
  
  std::vector<double> gradient(const std::vector<double>& par) const {
    
    double x = par[0];
    double y = par[1];
    double z = par[2];
    
    std::vector<double> result; result.reserve(3);
    result.push_back(2.*x);
    result.push_back(2.*y);
    result.push_back(2.*z);
    
    return result;  
  }

  void setErrorDef(double up) {theErrorDef = up;}

  double up() const {return theErrorDef;}

  const FCNBase* base() const {return this;}

//   virtual bool checkGradient() const {return false;}

private:
  double theErrorDef;
};

int main() {

  {
    //test constructor
    {
      // using migrad, numerical derivatives
      Quad3F fcn;
      MnUserParameters upar;
      upar.add("x", 1., 0.1);
      upar.add("y", 1., 0.1);
      upar.add("z", 1., 0.1);
      MnMigrad migrad(fcn, upar);
      FunctionMinimum min = migrad();
      std::cout<<"min= "<<min<<std::endl;
    }
    {
      // using VariableMetricMinimizer, analytical derivatives
      Quad3F fcn;
      std::vector<double> par;
      par.push_back(1.); par.push_back(1.); par.push_back(1.);
      std::vector<double> err;
      err.push_back(1.); err.push_back(1.); err.push_back(1.);
      VariableMetricMinimizer mini;
      FunctionMinimum min = mini.minimize(fcn, par, err);
      std::cout<<"min= "<<min<<std::endl;
    }

  }

  return 0;
}
