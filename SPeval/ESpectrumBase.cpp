/*!  @brief Some utility routines for the ESsistMe tool
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <math.h>
#include <algorithm>
#include <string>
#include "ESpectrumBase.h"
extern bool UNIT_TESTING;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// ESpectrumBase
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class ESpectrumBase
  \brief Represents 2-dim measured data in form of double-precision vectors

  The base class really stores the data, i.e. other objects can receive pointers to the data.
*/
    ESpectrumBase::
ESpectrumBase(void)
{
/*mData.resize(Y->size()); double x = 0.L;
std::vector<ESpectrumPoint>::iterator Dit = mData.begin();
for (std::vector<double>::iterator Yit = Y->begin(); Yit!=Y->end(); ++Yit, ++Dit)
{
  Dit->Y = *Yit; Dit->X = x; x += 1.L;  // Use a sequence number as "energy"
  Dit->dYR = Dit->Y >= 1 ? sqrt(Dit->Y) : 1;   // Protect from zero uncertainty
  Dit->dYR = 1/Dit->dYR;  // For efficiency, stores 1/dY
  Dit->Fit = 0.L;       // Initialize fitted value to 0
  */
     InitializeFunctionPointers();   // Set up function pointers
}

/*! \fn ESpectrumBase::ESpectrumBase(void)
 *File constructor for the 2-dim spectrum: data will be read from the file
 *Before using the constructor, member function FileMatchesTemplate must be called
*/
    ESpectrumBase::
ESpectrumBase(const std::string& FileName)
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
    InitializeFunctionPointers();   // Set up function pointers
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
    InitializeFunctionPointers();   // Set up function pointers
}

 /*! \brief ESpectrumBase::InitializeFunctionPointers*/
 /*!    \fn ESpectrumBase::InitializeFunctionPointers
  * @details The energy can be given as either kinetic or binding energy
  *
  * Using function pointers, the working energy scale can be chosen as either of the two,
  * provided that the excitation energy is known
  * 
  * Initially, kinetic energy scale is assumed. If the excitation energy is not known,
  * only the type of energy is changed, but not the calculation mode
  */
void ESpectrumBase::
InitializeFunctionPointers(void)
{
    //http://stackoverflow.com/questions/30228452/function-pointer-within-class
    ESpectrumBase::Ptr_X_Get = &ESpectrumBase::X_Get_Direct;   // Anyhow, be sure that XGet pointers have a value
    ESpectrumBase::Ptr_X_Get_Direct = &ESpectrumBase::X_Get_Direct;   //
    ESpectrumBase::Ptr_X_Get_Indirect = &ESpectrumBase::X_Get_Indirect;   // Until we know X value, use kinetic scale
    ESpectrumBase::Ptr_X_Get_Kinetic = &ESpectrumBase::X_Get_Direct;   //
    ESpectrumBase::Ptr_X_Get_Binding = &ESpectrumBase::X_Get_Direct;   // Until we know X value, use kinetic scale
    //
    mBinding = false;   // Initially, there is no binding scale
    mXEnergy = -1;      // Initially, there is no excitation energy
    mEFermi = 0;    // The work function is assumed to be 0
}

/*!
     * \brief ESpectrumBase::X_Get_Direct
     * \param[in] i kinetic energy at measured point i
     * \return
     */

/*!
 \f$ E_b = h*\nu - E_k - E_F \f$

\f$ E_k = h*\nu - E_b - E_F \f$
*/
    double ESpectrumBase::
X_Get_Direct(int i){
        return mData[i].X;
    };
    double ESpectrumBase::
X_Get_Indirect(int i){
        return mXEnergy- mData[i].X -mEFermi;
    };


    double
ESpectrumBase::X_Get(int i)
    {
        return (this->*Ptr_X_Get)(i);
    }


    double ESpectrumBase::
X_Get_Binding(int i)
   {
        return (this->*Ptr_X_Get_Binding)(i);
   }
   // Return X on binding scale
   double ESpectrumBase::
X_Get_Kinetic(int i)
   {
       return (this->*Ptr_X_Get_Kinetic)(i);
   }

   void ESpectrumBase::
XEnergy_Set(double XE)
{   mXEnergy = XE;
   Binding_Scale_Set(mBinding);
}

    bool ESpectrumBase::
Binding_Scale_Get(void)
    {
        return mBinding;
    }

    /*!
     * \brief ESpectrumBase::Binding_Scale_Set
     * \param Binding if to set to data base energy scale to binding
     * 
     * If the X energy is not valid, only the scale type is changed.
     * If the X energy is known, the energy values are calculated to the other energy type using 
     * 
     *  \f$ E_b = h*\nu - E_k - E_F \f$ or \f$ E_k = h*\nu - E_b - E_F \f$
     * 
     * Anyhow, the energy values will stay increasing
     */    
    void
    ESpectrumBase::
Binding_Scale_Set(bool Binding)
    {
        if(XEnergy_Valid())
        {// check if we need to change the scale type
            if(Binding ^ mBinding)
            {// we really need to change the scale type
                // The calculation is the same:
                int NP = mData.size()-1;
                int NPoints = (NP+3)/2; ESpectrumPoint DLow, DHigh;
                for(int i =0; i<NPoints; i++)
                {   // Calculate the new energy and replace element physically
                    DLow = mData[i]; DHigh = mData[NP-i];
                    DLow.X = EnergyOnOtherScale(DLow.X); DHigh.X = EnergyOnOtherScale(DHigh.X);
                    mData[i] = DHigh; mData[NP-i] = DLow;
                }
                mBinding = Binding;
            }
            if(mBinding)
            {
                ESpectrumBase::Ptr_X_Get_Kinetic = &ESpectrumBase::X_Get_Indirect;
                ESpectrumBase::Ptr_X_Get_Binding = &ESpectrumBase::X_Get_Direct;
            }
            else
            {
                ESpectrumBase::Ptr_X_Get_Kinetic = &ESpectrumBase::X_Get_Direct;
                ESpectrumBase::Ptr_X_Get_Binding = &ESpectrumBase::X_Get_Indirect;
            }

        }
        else
        {// Only the type is changed, nothing to calculate
            ESpectrumBase::Ptr_X_Get_Kinetic = &ESpectrumBase::X_Get_Direct;
            ESpectrumBase::Ptr_X_Get_Binding = &ESpectrumBase::X_Get_Direct;

        }
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
    /*!
     * \brief ESpectrumBase::EnergyToPoint
     * \param E the energy we are looking for
     * \param low the lower index
     * \param high the higher index
     * \return the index of the point searched
     *
     * Makes a binary search for the energy value E, returns the idex of the nearest value
     */

    int ESpectrumBase::
IndexOfEnergy(double E, int low, int high)
    {
        int first = 0; int last= high ? high : mData.size()-1;
        if(E<mData[first].X) return -1; // Too small value
        if(E>mData[last].X) return -1; // Too large value
        int step = 1; int it;
        while (step>0)
        {
          step = (last-first)/2; it = first + step;
          if (mData[it].X>E)
              last=it;
          else
             first= it;
        }
        return (abs(mData[last].X-E) < abs(mData[first].X-E)) ? last : first;
    }

    double ESpectrumBase::
EnergyOnOtherScale(double E)
        {return mXEnergy<0 ? E : mXEnergy- E -mEFermi;}

    int ESpectrumBase::
IndexOfKineticEnergy(double E, int low, int high)
    {
        if(mBinding)
            E = EnergyOnOtherScale(E);
        return IndexOfEnergy(E, low, high);
    }

    int ESpectrumBase::
IndexOfBindingEnergy(double E, int low, int high)
    {
        if(!mBinding)
            E = EnergyOnOtherScale(E);
        return IndexOfEnergy(E, low, high);
    }


/*    /// This function should return true if some specific spectrum data format is detected
    bool    SpectrumTemplate::FileMatchesTemplate(const wxString& path)
    {
        if(!wxFile::Exists(path))
            return false;   // Do not try if does not exist ...
        bool    Found = false;
        for (size_t i = 0; i<EWAFileFormats.GetCount(); i++)
        {   TheSpectrum = (SpectrumBase*)EWAFileFormats.Item(i)->Data();
            if (m_docClassInfo)
            {   SpectrumDocument *doc = (SpectrumDocument *)m_docClassInfo->CreateObject();
                doc->SetFilename(path);
                doc->SetDocumentName(GetDocumentName());
                EWADocManager *M = (EWADocManager *)GetDocumentManager();
                wxNode *N = M->GetTemplates()->GetFirst();
                int TempIndex = 0;
                while(N)
                {   // We have more templates
                    M->SetTemplateIndex(TempIndex++);   // Save template index for view creation
                    SpectrumTemplate* T = (SpectrumTemplate*)N->GetData();
                    doc->SetDocumentTemplate(T);
                    doc->SetCommandProcessor(NULL);
                    TheSpectrum->SetDocument( doc);
    //                LogMessage("Checking template (base)"+TheSpectrum->GetClassName(),false);
                    if(TheSpectrum->FileMatchesTemplate(path,T->GetViewClassInfo()))
                        { Found = true; break; }
    //                LogMessage(" failed");
                    N = N->GetNext(); // Get next template
                }
                delete  doc;
                if(Found) break;
    / *      stream.seekg(0,ios::beg);
            if(thisdoc->FormatDetected((istream)wxZlibInputStream(stream)))
             {   Found = true; break; }
    * /
            }
        }
        return Found;
    }//SpectrumTemplate::FileMatchesTemplate

*/
