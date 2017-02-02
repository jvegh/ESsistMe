/*! @brief SpectrumBase for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef ESsistMe_ESpectrum1Dim_H
#define ESsistMe_ESpectrum1Dim_H
#include <string>
#include <iostream>
#include <vector>
#include "ESpectrumBase.h"
#include "DataIO.h"
#include "EComponentBase.h"
using namespace std;
/*! @brief Base class for storing spectral data */
class ESpectrum1Dim :public 	ESpectrumBase, public DataIO
{
public:
  ESpectrum1Dim(void);
  ESpectrum1Dim(const std::string& FileName);
  ESpectrum1Dim(vector<double> *Y);
  ESpectrum1Dim(vector<double> *X, vector<double> *Y);
  ESpectrum1Dim(vector<double> *X, vector<double> *Y, vector<double> *dY);

protected:
};

#endif //  ESsistMe_ESpectrumBase_H

#if defined(OldX)
//: spectr1.h
 */

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "spectr1.h"
#endif

#ifndef ewa_spectr1h
#define ewa_spectr1h

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class Component1Dim;
#include    "specbase.h"
#include    "compbase.h"

/// 1-dimensional spectrum class
class Spectrum1Dim : public SpectrumBase
{
    DECLARE_DYNAMIC_CLASS(Spectrum1Dim)
protected:
    bool Calibrated;                    ///< true if the spectrum is calibrated
    float DependenceNo;                 ///< Dependence parameter, if any
    int SpectrumNo;                     ///< Number of the spectrum
    float   *CData,                     ///< The dependent variable, usually counts
            *dCData,                    ///< its uncertainty
            *EffData,                   ///< The efficiency, if present
            *FitData;                   ///< The fitted equivalent of YData
    float   **CArray,                   ///< Pointers to Y data in individual Spectra
            **dCArray,                  ///<             their uncertainties
            **FitArray,                 ///<             the fitted equivalents
            **EffArray;                 ///<             their efficiencies
public:
    /// Ctor for 1-dim spectrum
    Spectrum1Dim(void);
    /// Copy ctor for 1-dim spectrum
    Spectrum1Dim(const Spectrum1Dim &S);
    /// Dtor for 1-dim spectrum
    virtual ~Spectrum1Dim();
    /// Return a cloned copy of the spectrum
    virtual SpectrumBase *Clone(void) const
        {   return new Spectrum1Dim(*this);}
    /// Fill up data for a real spectrum from a document
    bool    Create(SpectrumDocument *doc);
    /// Returns 'true' if file is of format Self
    virtual bool FileMatchesTemplate(const wxString& path, wxClassInfo* CI);
    /// Return pointer to the active component
    Component1Dim*  GetActiveComponent(void)
        {   return (Component1Dim*)SpectrumBase::GetActiveComponent();}
  // Access spectral data
    /** Return pointer to measurement count data. \see SetCountData */
    inline float*   GetCountData(void) const
        {   return CData;}
    /** Set pointer to measurement count data. \see GetCountData */
    inline   void SetCountData(float* CD)
        {   CData = CD;}
    /// Get the address of the dataset pointers
    inline float**    GetDatasets()
        {   return  CArray;}
    /** Return the count value at i. \see SetC */
    inline float   GetC(const int i) const
        {   return CData[i];}
    /** Set count at i. \see GetC */
    inline void SetC(const int i, const float v)
        {   CData[i] = v;}
    /** Get uncertainty of the count at i. \see SetdC */
    inline float   GetdC(const int i) const
        {   return dCData[i];}
    /// Set uncertainty of count at i
    inline void    SetdC(const int i, const float v)
        {   dCData[i] = v;}
    /// Return pointer to efficiency data
    inline float*   GetEffData(void) const
        {   return EffData;}
    /// Return efficiency at i
    virtual float   GetEff(int i)
        {   return EffData[i];}
    /// Set efficiency at i
    inline void    SetEff(int i, float v)
        {   EffData[i] = v;}
    /// Return pointer to fitted data
    inline float*   GetFitData(void) const
        {   return FitData;}
    /// Return fitted data at i
    inline float   GetFit(int i)
        {   return FitData[i];}
    /// Set fitted data at i
    inline void    SetFit(int i, float v)
        {   FitData[i] = v;}
    /// Set uncertainty to v, with uncertainty dv
    void    SetUncertainty(int i, float v, float dv = -1.0);
    /// Return the net count at i
    virtual float   GetNetC(int i)
        {return FitData ? GetC(i)-GetFit(i) :GetC(i);}
    /// Return pointer to component, given by its ID or NULL if not found
//    Component1Dim*  GetComponentByID(int N)
//        {   return  (Component1Dim*) SpectrumBase::GetComponentByID(N);}
    /// Return max count between low and high
    float   GetMaxCBetween(int Low, int High, bool WithError=false);
    /// Return min count between low and high
    float   GetMinCBetween(int Low, int High, bool WithError=false);
    /// Activate a dataset
    virtual void    ActivateDataset(const size_t Index);
    /// Return number of attached components
    inline int GetNoOfComponents(void)
        {return Components->GetCount();}
    /// Return sum of squares
    virtual double GetSumOfSquares(void);
    /// Return a descriptive text
    virtual void    GetText(wxStringList **T);
    /// Return background text
    virtual void    GetTextBackground(wxStringList **T);
    /// Return fit text
    virtual void    GetTextFit(wxStringList **T);
    /// Return measurement text
    virtual void    GetTextMeas(wxStringList **T);
    /// Return list of components
    virtual void GetTextComponentList(wxStringList **T);
    /// Return peak text
    virtual void    GetTextPeak(wxStringList **T);
    /// Return pseudocomponent text
    void GetTextPseudocomponents(wxStringList **T);
    /// Return a summary table for peaks
    void GetTextPeakTable(wxStringList **T);
/*  void    CreateEfficiency();
*/
    /// Return true if efficiency data is present
    inline bool IsEfficiencyPresent()
        {return EffData!=NULL;}
    /// Return number of total and active parameters
/*    void    GetNoOfParameters(size_t &Total, size_t &Active)
        {   GetRoot()->GetNoOfParameters(Total,Active);}
*/
    /// All algorithms assume ascending E scale, so revert it if not so
    void MakeAscendingScale(void);
    /// Read count data, one by one, from the stream
    bool    ReadASCIIData(istream& stream);
    /// Read calibration data from file
    bool    ReadCalibrationData(const wxString& path);
    /// Remove a dataset
    void    RemoveDataset(const size_t ix);
    /// Reserve space for the fittede data
    virtual void    ReserveFitData();
    /// Make initial, post-loading setup
    virtual bool    Setup();
    /// Sup up attached components
    void    SumUpComponents(void);
    /// Write file data to disk file
    bool   WriteDiskFile(wxString S);
    // These values are present once per data set
};// of class Spectrum1Dim

void InitFileFormats1Dim();

void SpectrumSaveComponents(SpectrumView *w);

#endif // ewa_spectr1h
#endif
