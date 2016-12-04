#include "FunctionMinimum.h"
#include "MnPrint.h"
#include "VariableMetricMinimizer.h"
#include "MnMigrad.h"
#include "MnContours.h"

#include "FCNGradientBase.h"

class Quad2F : public FCNGradientBase {

public:

  Quad2F() : theErrorDef(1.) {}

  ~Quad2F() {}

  double operator()(const std::vector<double>& par) const {

    double x = par[0];
    double y = par[1];

    return ( x*x  + y*y );
  }
  
  std::vector<double> gradient(const std::vector<double>& par) const {
    
    double x = par[0];
    double y = par[1];
    
    std::vector<double> result; result.reserve(2);
    result.push_back(2.*x);
    result.push_back(2.*y);
    
    return result;  
  }

  void setErrorDef(double up) {theErrorDef = up;}

  double up() const {return theErrorDef;}

  const FCNBase* base() const {return this;}

private:
  double theErrorDef;

};

int main() {

  //test constructor
  {
    Quad2F fcn;
    MnUserParameters upar;
    upar.add("x", 1., 0.1);
    upar.add("y", 1., 0.1);
    MnMigrad migrad(fcn, upar);
    FunctionMinimum min = migrad();
    std::cout<<"min= "<<min<<std::endl;
  }
  {
    // using VariableMetricMinimizer, analytical derivatives
    Quad2F fcn;
    std::vector<double> par; par.push_back(1.); par.push_back(1.); 
    std::vector<double> err; err.push_back(0.1); err.push_back(0.1);
    VariableMetricMinimizer mini;
    FunctionMinimum min = mini.minimize(fcn, par, err);
    std::cout<<"min= "<<min<<std::endl;
  }
  {
    // test Contours for two parameters
    Quad2F fcn;
    std::vector<double> par; par.push_back(1.); par.push_back(1.); 
    std::vector<double> err; err.push_back(0.1); err.push_back(0.1);
    VariableMetricMinimizer mini;
    FunctionMinimum min = mini.minimize(fcn, par, err);
    MnContours contours(fcn, min);
    //1-sigma around the minimum
    std::vector<std::pair<double,double> > cont = contours(0, 1, 20);
    // the minimum
    std::cout<<"1-sigma contours"<<std::endl;
    for(std::vector<std::pair<double,double> >::iterator ipair =cont.begin();
	ipair != cont.end(); ipair++) {
      std::cout<<"x,y "<<(*ipair).first<<" "<<(*ipair).second<<std::endl;
    }
    
    //2-sigma around the minimum
    std::cout<<"2-sigma contours"<<std::endl;
    fcn.setErrorDef(4.);
    std::vector<std::pair<double,double> > cont4 = contours(0, 1, 20);
    for(std::vector<std::pair<double,double> >::iterator ipair=cont4.begin();
	ipair != cont4.end(); ipair++) {
      std::cout<<"x,y "<<(*ipair).first<<" "<<(*ipair).second<<std::endl;
    }  
  }
  
  return 0;
}
