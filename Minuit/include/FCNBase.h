#ifndef MN_FCNBase_H_
#define MN_FCNBase_H_

#include <vector>

/** User function base class, has to be implemented by the user. up() is the
    error definition of the function, e.g. is 1 if function is Chi2 and 
    0.5 if function is -logLikelihood. If the user wants instead the 2-sigma
    errors, up() = 4, as Chi2(x+n*sigma) = Chi2(x) + n*n.
 */

class FCNBase {

public:

  virtual ~FCNBase() {}

  virtual double operator()(const std::vector<double>&) const = 0;
  virtual double errorDef() const {return up();}
  virtual double up() const = 0;

};

#endif //MN_FCNBase_H_
