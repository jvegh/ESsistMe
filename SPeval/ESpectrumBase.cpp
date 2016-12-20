/*!  @brief Some utility routines for the ESsistMe tool
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <math.h>
#include "ESpectrumBase.h"
bool UNIT_TESTING;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// ESpectrumBase
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class ESpectrumBase
  \brief Represents 2-dim measured data in form of double-precision vectors

  This class acts as a replacement for QVector2D with the advantage of double precision instead of
  single, and some convenience methods tailored for the QCustomPlot library.
  The base class really stores the data, i.e. other objects can receive pointers to the data.
*/
  /*! \fn ESpectrumBase::ESpectrumBase(vector<double>* Y)
   * Single-vector constructor for the 2-dim spectrum
    Creates a base spectrum, using the intensity data only.
    The energy data are simple sequence numbers.
    The uncertainties are the square root of the intensity data
    (zero protection provided).
    The "energy" is simple a sequence number, \see Calibrate
  */
    ESpectrumBase::
ESpectrumBase(vector<double>* Y)
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

    ESpectrumBase::
ESpectrumBase(vector<double>*X, vector<double>*Y)
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
}

    ESpectrumBase::
ESpectrumBase(vector<double>*X, vector<double>*Y, vector<double>*dY)
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
}

 /*! \brief ESpectrumBase::InitializeFunctionPointers*/
 /*!    \fn ESpectrumBase::InitializeFunctionPointers
  * @details The energy can be given as either kinetic or binding energy
  *
  * Using function pointers, the working energy scale can be chosen as either of the two,
  * provided that the excitation energy is known
  */
void ESpectrumBase::
InitializeFunctionPointers(void)
{
    //
    ESpectrumBase::Ptr_X_Get = &ESpectrumBase::X_Get_Kinetic;   // Anyhow, be sure that XGet pointers have a value
    ESpectrumBase::Ptr_X_Get_Kinetic = &ESpectrumBase::X_Get_Kinetic;   //
    ESpectrumBase::Ptr_X_Get_Binding = &ESpectrumBase::X_Get_Kinetic;   // Until we know X value, use kinetic scale
    //
    mBinding = false;   // Initially, there is no binding scale
    mXEnergy = -1;      // Initially, there is no excitation energy
}

/*!
     * \brief ESpectrumBase::X_Get_Kinetic
     * \param[in] i kinetic energy at measured point i
     * \return
     */

/*!
 \f$ E_b = h*\nu - E_k - E_F \f$

\f$ E_k = h*\nu - E_b - E_F \f$
*/
    double ESpectrumBase::
   X_Get_Kinetic(int i){ // if(i < 0) i = 0; if (i>= mNoOfPoints ) i = mNoOfPoints;
        return mData[i].X;
//        std::cout << "Kinetic" << std::endl; double di; di = i+5;
//        return di;
    };
    double ESpectrumBase::
   X_Get_Binding(int i){
        return mXEnergy- mData[i].X;
//        std::cout << "Binding" << std::endl; double di; di = i; return di;
    };


    double
ESpectrumBase::X_Get(int i)
    {
        return (this->*Ptr_X_Get)(i);
    }


    bool
ESpectrumBase::Binding_Scale_Get(void)
    {
        return true;
    }

    void
ESpectrumBase::Binding_Scale_Set(bool Binding)
    {

    }

    /*! \brief  Makes a simple linear energy calibration */
    /*!    \fn ESpectrumBase::Calibrate(double Intercept, double Slope)
{
     * \param[in] Intercept of the calibrating line
     * \param[in] Slope of the calibrating line
     */
    void ESpectrumBase::
Calibrate(double Intercept, double Slope)
{
    for (std::vector<ESpectrumPoint>::iterator it = mData.begin(); it!=mData.end(); ++it)
        {
          it->X =  Intercept + Slope*it->X;
        }
}


    double ESpectrumBase::
ChiSq_Get(int i)
{
    double chisq = 0.L; double ch; int N = mData.size();
//    for (std::vector<ESpectrumPoint>::iterator it = mData.begin(); it!=mData.end(); ++it)
    for(int it = 0; it<N; it++)
        {
            ch = Residual_Get(it);
            chisq += ch*ch;
        }
    return chisq;
    }

/*
     double
ESpectrumBase::X_Get_Binding(int i)
    {
        }
    // Return X on binding scale
    double
ESpectrumBase::X_Get_Kinetic(int i)
    {
    }
*/

#if 0
/*!
  Creates a QCPVector2D object and initializes the \a x and \a y coordinates with the specified
  values.
*/
ESpectrumBase::ESpectrumBase(double x, double y) :
  mX(x),
  mY(y)
{
}

/*!
  Creates a QCPVector2D object and initializes the x and y coordinates respective coordinates of
  the specified \a point.
*/
ESpectrumBase::ESpectrumBase(const QPoint &point) :
  mX(point.x()),
  mY(point.y())
{
}

/*!
  Creates a QCPVector2D object and initializes the x and y coordinates respective coordinates of
  the specified \a point.
*/
QCPVector2D::QCPVector2D(const QPointF &point) :
  mX(point.x()),
  mY(point.y())
{
}


#endif //0
