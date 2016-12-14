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


class ESpectrumBase
{
public:
  ESpectrumBase();
/*  SpectrumBase(double x, double y);
  */
  double X_Get(int i);     // Return in the actual scale mode
  double X_Get_Binding(int i);     // Return X on binding scale
  double X_Get_Kinetic(int i);     // Return X on
  bool Binding_Scale_Get(void);
  void Binding_Scale_Set(bool Binding=false);
  void XEnergy_Set(double XE){ mXEnergy = XE;}
  double XEnergy_Get(void){ return mXEnergy;}
  bool XEnergy_Valid(void){ return mXEnergy>0;}
private:
  double mX, mY;
  bool mBinding;    // If the energy values are in binding scale
  double mXEnergy;
  double (ESpectrumBase::*Ptr_X_Get)(int i);
  double (ESpectrumBase::*Ptr_X_Get_Binding)(int i);
  double (ESpectrumBase::*Ptr_X_Get_Kinetic)(int i);
};


#endif //  ESsistMe_ESpectrumBase_H
