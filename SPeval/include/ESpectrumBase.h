/*! @brief SpectrumBase for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef ESsistMe_ESpectrumBase_H
#define ESsistMe_ESpectrumBase_H
#include <string>
#include <iostream>
#include <vector>
using namespace std;

// To provide good locality, variables relating to a data point are kept in a record
struct ESpectrumPoint {
    double X;   // The independent variable
    double Y;   // The dependent variable
    double dYR;  // Their uncertainty
    double Fit; // The fitted value
};

/*! @brief Base class for storing spectral data */
class ESpectrumBase
{
public:
  ESpectrumBase(vector<double> *Y);
  ESpectrumBase(vector<double> *X, vector<double> *Y);
  ESpectrumBase(vector<double> *X, vector<double> *Y, vector<double> *dY);
  double X_Get(int i);     // Return X in the actual scale mode
  double X_Get_Binding(int i);     // Return X on binding scale
  double X_Get_Kinetic(int i);     // Return X on
  bool Binding_Scale_Get(void);
  void Binding_Scale_Set(bool Binding=false);
  void XEnergy_Set(double XE);
  double XEnergy_Get(void){ return mXEnergy;}
  bool XEnergy_Valid(void){ return mXEnergy>0;}
  int NoOfDataPoints_Get(void){ return mData.size(); }
  double Y_Get(int i) { return mData[i].Y;}
  double dY_Get(int i) { return 1/mData[i].dYR;}
  double dYR_Get(int i) { return mData[i].dYR;}
  double Fit_Get(int i) { return mData[i].Fit;}
  double Residual_Get(int i){ return (mData[i].Fit-mData[i].Y)*mData[i].dYR;}
  double ChiSq_Get(int i);
  double EnergyOnOtherScale(double E);
  void Calibrate(double Intercept, double Slope);
  int IndexOfEnergy(double E, int low=0, int high=0);
  int IndexOfKineticEnergy(double E, int low=0, int high=0);
  int IndexOfBindingEnergy(double E, int low=0, int high=0);
  int Size_Get(void){ return mData.size();}
private:
    void
  InitializeFunctionPointers(void);
  vector<ESpectrumPoint> mData;   //< Store energy, intensity and uncertainty
  bool mBinding;    // If the energy values are in binding scale
  double mXEnergy;
  double mEFermi;
  double X_Get_Direct(int i);     // Return X without calculation
  double X_Get_Indirect(int i);     // Return X with calculation
  double (ESpectrumBase::*Ptr_X_Get)(int i);
  double (ESpectrumBase::*Ptr_X_Get_Direct)(int i);
  double (ESpectrumBase::*Ptr_X_Get_Indirect)(int i);
  double (ESpectrumBase::*Ptr_X_Get_Kinetic)(int i);
  double (ESpectrumBase::*Ptr_X_Get_Binding)(int i);
};


#endif //  ESsistMe_ESpectrumBase_H
