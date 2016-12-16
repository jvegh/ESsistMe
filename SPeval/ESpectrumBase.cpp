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
*/

/*! \details Base class for storing spectral data
 *  The base class really stores the data, i.e.
*/

  /*! \fn Single-vector constructor for the 2-dim spectrum
    Creates a base spectrum, using the intensity data only.
    The energy data are simple sequence numbers.
    The uncertainties are the square root of the intensity data
    (zero protection provided).
  */
    ESpectrumBase::
ESpectrumBase(vector<double>* Y)
{
    mData.resize(Y->size()); double x = 0.L;
    cerr << NoOfDataPoints_Get() << " points" << endl;
    std::vector<ESpectrumPoint>::iterator Dit = mData.begin();
    for (std::vector<double>::iterator Yit = Y->begin(); Yit!=Y->end(); ++Yit, ++Dit)
    {
      Dit->Y = *Yit; Dit->X = x; x += 1.L;
      *Yit >= 1 ? sqrt(*Yit) : 1;   // Protect from zero uncertainty
      Dit->dY = *Yit;
      std::cerr << ' ' << *Yit << ' ' << Dit->Y;
    }

    InitializeFunctionPointers();   // Set up function pointers
/*    cerr << (this->*Ptr_X_Get)(1);
    ESpectrumBase::Ptr_X_Get = &ESpectrumBase::X_Get_Binding;
    cerr << (this->*Ptr_X_Get)(2);
    ESpectrumBase::Ptr_X_Get = &ESpectrumBase::X_Get_Kinetic;
    cerr << (this->*Ptr_X_Get)(3) << "Again";
   cerr << "I am here" << endl;*/
}

    /*! \fn Double-vector constructor for the 2-dim spectrum
      Creates a base spectrum using the provided energy and intensity data
      the uncertainties are the square root of the intensity data

    */

    ESpectrumBase::
ESpectrumBase(vector<double>*X, vector<double>*Y)
{
}

    ESpectrumBase::
ESpectrumBase(vector<double>*X, vector<double>*Y, vector<double>*dY)
{
}

 /*! \brief ESpectrumBase::InitializeFunctionPointers*/
 /*!    \fn InitializeFunctionPointers
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

/*!
  Normalizes this vector. After this operation, the length of the vector is equal to 1.

  \see normalized, length, lengthSquared
*/
void QCPVector2D::normalize()
{
  double len = length();
  mX /= len;
  mY /= len;
}

#endif //0
