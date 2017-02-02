/*!  @brief Some utility routines for the ESsistMe tool
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <math.h>
#include <algorithm>
#include <string>
#include "ESpectrum1Dim.h"
extern bool UNIT_TESTING;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// ESpectrum1Dim
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class ESpectrum1Dim
  \brief Represents 2-dim measured data in form of double-precision vectors

  The base class really stores the data, i.e. other objects can receive pointers to the data.
*/
    ESpectrum1Dim::ESpectrum1Dim(void):
ESpectrumBase(), DataIO()
{
     InitializeFunctionPointers();   // Set up function pointers
}

/*! \fn ESpectrumBase::ESpectrumBase(void)
 *File constructor for the 2-dim spectrum: data will be read from the file
 *Before using the constructor, member function FileMatchesTemplate must be called
*/
    ESpectrum1Dim::ESpectrum1Dim(const std::string& FileName):
ESpectrumBase(FileName), DataIO()
{
/*mData.resize(Y->size()); double x = 0.L;
std::vector<ESpectrumPoint>::iterator Dit = mData.begin();
for (std::vector<double>::iterator Yit = Y->begin(); Yit!=Y->end(); ++Yit, ++Dit)
{
  Dit->Y = *Yit; Dit->X = x; x += 1.L;  // Use a sequence number as "energy"
  Dit->dYR = Dit->Y >= 1 ? sqrt(Dit->Y) : 1;   // Protect from zero uncertainty
  Dit->dYR = 1/Dit->dYR;  // For efficiency, stores 1/dY
  Dit->Fit = 0.L;       // Initialize fitted value to 0
}
*/
    InitializeFunctionPointers();   // Set up function pointers
}
  /*! \fn ESpectrumBase::ESpectrumBase(vector<double>* Y)
   * Single-vector constructor for the 2-dim spectrum
    Creates a base spectrum, using the intensity data only.
    The energy data are simple sequence numbers.
    The uncertainties are the square root of the intensity data
    (zero protection provided).
    The "energy" is simple a sequence number, \see Calibrate
  */
    ESpectrum1Dim::ESpectrum1Dim(vector<double>* Y):
ESpectrumBase(Y), DataIO()
{
    mData.resize(Y->size()); double x = 0.L;
    std::vector<ESpectrumPoint>::iterator Dit = mData.begin();
    for (std::vector<double>::iterator Yit = Y->begin(); Yit!=Y->end(); ++Yit, ++Dit)
    {
      Dit->Y = *Yit; Dit->X = x; x += 1.L;  // Use a sequence number as "energy"
      Dit->dYR = Dit->Y >= 1 ? sqrt(Dit->Y) : 1;   // Protect from zero uncertainty
      Dit->dYR = 1/Dit->dYR;  // For efficiency, stores 1/dY
      Dit->Fit = 0.L;       // Initialize fitted value to 0
    }

    InitializeFunctionPointers();   // Set up function pointers
}

    /*! \fn ESpectrumBase::ESpectrumBase(vector<double>*X, vector<double>*Y)
     * Double-vector constructor for the 2-dim spectrum
      Creates a base spectrum using the provided energy and intensity data
      the uncertainties are the square root of the intensity data

    */

    ESpectrum1Dim::ESpectrum1Dim(vector<double>*X, vector<double>*Y):
ESpectrumBase(X,Y),  DataIO()
{
    mData.resize(X->size());
    std::vector<ESpectrumPoint>::iterator Dit = mData.begin();
    std::vector<double>::iterator Xit = X->begin();
    for (std::vector<double>::iterator Yit = Y->begin(); Yit!=Y->end(); ++Yit, ++Xit, ++Dit)
        {
          Dit->Y = *Yit; Dit->X = *Xit;
          Dit->dYR = Dit->Y >= 1 ? sqrt(Dit->Y) : 1;   // Protect from zero uncertainty
          Dit->dYR = 1/Dit->dYR;  // For efficiency, stores 1/dY
          Dit->Fit = 0.L;       // Initialize fitted value to 0
        }
    InitializeFunctionPointers();   // Set up function pointers
}

    ESpectrum1Dim::ESpectrum1Dim(vector<double>*X, vector<double>*Y, vector<double>*dY):
ESpectrumBase(X,Y,dY),  DataIO()
{
    mData.resize(X->size());
    std::vector<ESpectrumPoint>::iterator Dit = mData.begin();
    std::vector<double>::iterator Xit = X->begin();
    std::vector<double>::iterator dYit = dY->begin();
    for (std::vector<double>::iterator Yit = Y->begin(); Yit!=Y->end(); ++Yit, ++Xit, ++dYit, ++Dit)
        {
              Dit->Y = *Yit; Dit->X = *Xit; Dit->dYR = *dYit;
              if(Dit->dYR < 1) Dit->Y = 1;   // Protect from zero uncertainty
              Dit->dYR = 1/Dit->dYR;  // For efficiency, stores 1/dY
              Dit->Fit = 0.L;       // Initialize fitted value to 0
        }
    InitializeFunctionPointers();   // Set up function pointers
}

