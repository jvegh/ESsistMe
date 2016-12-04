#ifndef MN_FCNMonitorBase_H_
#define MN_FCNMonitorBase_H_

#include "MinimumSeed.h"
//#include <vector>

/** Minimization monitorize function base class, has to be implemented by the user. 
	It is called when one iteration step finished (i.e. some local minimum found)
 */

class FCNMonitorBase {

public:

  virtual ~FCNMonitorBase() {}

  virtual void operator()(const int CurrentIter, ///< Current iteration count
							const int IterFreq,  ///< How frequently to react
							const int NoOfCalls,	///< Number of function calls made
							const double fval,	 ///< The present minimum function value
							const MinimumState &s0) const = 0;
};


#endif //MN_FCNMonitorBase_H_
