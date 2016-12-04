#ifndef MN_VariableMetricBuilder_H_
#define MN_VariableMetricBuilder_H_

#include "MinimumBuilder.h"
#include "VariableMetricEDMEstimator.h"
#include "DavidonErrorUpdator.h"

class VariableMetricBuilder : public MinimumBuilder {

public:

  VariableMetricBuilder() : theEstimator(VariableMetricEDMEstimator()), 
			    theErrorUpdator(DavidonErrorUpdator()) {}

  ~VariableMetricBuilder() {}

  virtual FunctionMinimum minimum(const MnFcn&, const GradientCalculator&, const MinimumSeed&, const MnStrategy&, unsigned int, double) const;

  FunctionMinimum minimum(const MnFcn&, const GradientCalculator&, const MinimumSeed&, unsigned int, double) const;

  const VariableMetricEDMEstimator& estimator() const {return theEstimator;}
  const DavidonErrorUpdator& errorUpdator() const {return theErrorUpdator;}

private:

  VariableMetricEDMEstimator theEstimator;
  DavidonErrorUpdator theErrorUpdator;
};

#endif //MN_VariableMetricBuilder_H_
