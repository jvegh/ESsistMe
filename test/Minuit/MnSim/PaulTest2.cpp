#include "GaussFcn.h"
#include "FunctionMinimum.h"
#include "MnMigrad.h"
#include "MnMinos.h"
#include "MnPrint.h"

#include <iostream>
#include <fstream>

int main() {

  std::vector<double> positions;
  std::vector<double> measurements;
  std::vector<double> var;
  double nmeas = 0;
  std::ifstream in("paul2.txt");
//   std::ofstream xout("x_2.txt");
//   std::ofstream yout("y_2.txt");
//   std::ofstream eout("e_2.txt");

  {
  double x = 0., y = 0., width = 0., err = 0., un1 = 0., un2 = 0.;
  while(in>>x>>y>>width>>err>>un1>>un2) {
    if(err < 1.e-8) continue;
    positions.push_back(x);
    measurements.push_back(y);
    var.push_back(err*err);
    nmeas += y;
//     xout<<x<<", ";
//     yout<<y<<", ";
//     eout<<err*err<<", ";
  }
  std::cout<<"size= "<<var.size()<<std::endl;
  assert(var.size() > 0);

  std::cout<<"nmeas: "<<nmeas<<std::endl;
  }
  // create FCN function  
  GaussFcn theFCN(measurements, positions, var);

  std::vector<double> meas = theFCN.measurements();
  std::vector<double> pos = theFCN.positions();

  // create initial starting values for parameters
  double x = 0.;
  double x2 = 0.;
  double norm = 0.;
  double area = 0.;
  double dx = pos[1]-pos[0];
  for(unsigned int i = 0; i < meas.size(); i++) {
    norm += meas[i];
    x += (meas[i]*pos[i]);
    x2 += (meas[i]*pos[i]*pos[i]);
    area += dx*meas[i];
  }
  double mean = x/norm;
  double rms2 = x2/norm - mean*mean;

  std::cout<<"initial mean: "<<mean<<std::endl;
  std::cout<<"initial sigma: "<<sqrt(rms2)<<std::endl;
  std::cout<<"initial area: "<<area<<std::endl;
  std::vector<double> init_val(3);
  init_val[0] = mean;
  init_val[1] = sqrt(rms2);
  init_val[2] = area; 
  std::cout<<"initial fval: "<<theFCN(init_val)<<std::endl;
  
  MnUserParameters upar;
  upar.add("mean", mean, 1.);
  upar.add("sigma", sqrt(rms2), 1.);
  upar.add("area", area, 10.);

  MnMigrad migrad(theFCN, upar);
  std::cout<<"start migrad "<<std::endl;
  FunctionMinimum min = migrad();
  std::cout<<"minimum: "<<min<<std::endl;

  std::cout<<"start minos"<<std::endl;
  MnMinos minos(theFCN, min);
  std::pair<double,double> e0 = minos(0);
  std::pair<double,double> e1 = minos(1);
  std::pair<double,double> e2 = minos(2);
  
  std::cout<<"par0: "<<min.userState().value("mean")<<" "<<e0.first<<" "<<e0.second<<std::endl;
  std::cout<<"par1: "<<min.userState().value(1)<<" "<<e1.first<<" "<<e1.second<<std::endl;
  std::cout<<"par2: "<<min.userState().value(2)<<" "<<e2.first<<" "<<e2.second<<std::endl;

  return 0;
}
