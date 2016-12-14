/*!  @brief Some utility routines for the ESsistMe tool
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include "ESpectrumBase.h"
bool UNIT_TESTING;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// ESpectrumBase
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class ESpectrumBase
  \brief Represents two doubles as a mathematical 2D vector

  This class acts as a replacement for QVector2D with the advantage of double precision instead of
  single, and some convenience methods tailored for the QCustomPlot library.
*/

/*!
  Creates a base spectrum object
*/
ESpectrumBase::ESpectrumBase() :
  mX(0),
  mY(0),
  mBinding(false),   // Initially, there is no binding scale
  mXEnergy(-1)  // Initially, there is no excitation energy
{
    ESpectrumBase::Ptr_X_Get = &ESpectrumBase::X_Get_Kinetic;   // Anyhow, be sure that XGet pointers have a value
    ESpectrumBase::Ptr_X_Get_Kinetic = &ESpectrumBase::X_Get_Kinetic;   //
    ESpectrumBase::Ptr_X_Get_Binding = &ESpectrumBase::X_Get_Kinetic;   // Until we know X value, use kinetic scale

    cerr << (this->*Ptr_X_Get)(1);
    ESpectrumBase::Ptr_X_Get = &ESpectrumBase::X_Get_Binding;
    cerr << (this->*Ptr_X_Get)(2);
    ESpectrumBase::Ptr_X_Get = &ESpectrumBase::X_Get_Kinetic;
    cerr << (this->*Ptr_X_Get)(3) << "Again";
   cerr << "I am here" << endl;
}

    double ESpectrumBase::
   X_Get_Kinetic(int i){ std::cout << "Kinetic" << std::endl; double di; di = i+5; return di;};
    double ESpectrumBase::
   X_Get_Binding(int i){ std::cout << "Binding" << std::endl; double di; di = i; return di;};


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
