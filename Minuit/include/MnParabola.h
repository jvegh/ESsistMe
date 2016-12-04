#ifndef MN_MnParabola_H_
#define MN_MnParabola_H_

#include <math.h>

/** parabola = a*xx + b*x + c
 */

class MnParabola {

public:

  MnParabola(double a, double b, double c) : theA(a), theB(b), theC(c) {}

  ~MnParabola() {}

  double y(double x) const {return (theA*x*x + theB*x +theC);}
  double x_pos(double y) const {return (sqrt(y/theA + min()*min() - theC/theA) + min());}
//   double x_pos(double y) const {return (sqrt(y/theA + theB*theB/(4.*theA*theA) - theC/theA)  - theB/(2.*theA));}
  double x_neg(double y) const {return (-sqrt(y/theA + min()*min() - theC/theA) + min());}
  double min() const {return -theB/(2.*theA);}
  double ymin() const {return (-theB*theB/(4.*theA) + theC);}

  double a() const {return theA;}
  double b() const {return theB;}
  double c() const {return theC;}

private:

  double theA;
  double theB;
  double theC;
};
#endif //MN_MnParabola_H_
