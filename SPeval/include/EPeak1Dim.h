/*! @brief ComponentBase for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef ESsistMe_Epeak1Dim_H
#define ESsistMe_EPeak1Dim_H
#include <string>
#include <iostream>
#include <vector>
#include "EComponent1Dim.h"
#include "ESpectrum1Dim.h"
//#include "MinuitParameter.h"
using namespace std;


/*! @brief Base class for 1-dimensional spectrum components */
class EPeak1Dim : public EComponent1Dim
{
public:
  EPeak1Dim (ESpectrum1Dim *aSpectrum, int aWIdent,  const string& aLabel);
protected:
  ESpectrumBase    *m_Spectrum;   ///< Back pointer to the owner spectrum, a copy
};


#endif //  ESsistMe_EComponentBase_H
#if defined(OldPeak1)
//: peak1.h
/*  Peak -in  1 dimension type, base stuff
    \author   (c) JÃ¡nos VÃ©gh, MTA ATOMKI, Debrecen, Hungary (veghj@users.sf.net)
    \since    Dec 02, 2004
    $Id: peak1.h,v 1.1 2004-07-12 11:07:48+02 veghj Exp veghj $
 */

#if defined(__GNUG__) && !defined(__APPLE__)
        #pragma interface "peak1.h"
#endif

#ifndef ewa_peakh
#define ewa_peakh

#include	"spectr1.h"
#include	"comp1.h"

const double ln2 = 0.693147180559945;
const double sqrtln2 = 0.832554611157698;
const double PI = 3.14159;

class	PeakTailType;

/// 1-dimensional peak class
class Peak1Dim : public Component1Dim
{
    DECLARE_DYNAMIC_CLASS(Peak1Dim)
public:
    /// Ctor for 1-dim peaks
        Peak1Dim(Spectrum1Dim *aSpectrum, const int aWIdent,  wxString aLabel);
    /// Default ctor for 1-dim peaks
        Peak1Dim();
    /// Copy ctor for 1-dim peaks
        Peak1Dim(const Peak1Dim &P) ;
    /// Select a component from the subtype list and create it
    virtual int     Create(int CommandCode, wxList *aTypes, size_t &cindex);
    /// Return the area to height conversion factor
    virtual double AreaToHeight()
        {   return 1.0;};
    /// Calculate the peak contribution
        virtual	void Calculate(void);
    /// Create a component dialog for parametrization
    virtual ComponentDialog* CreateComponentDialog(SpectrumView *view);
    /// Prepare getting parameters from the dialog
        virtual void	CreateGetParameters(void);
    /// Prepare setting parameters for the dialog
        virtual void	CreateSetParameters(void);
    /// Return the area of the peak
        double GetArea(void);
    /// Return energy of the peak
        double GetEnergy(void);
    /// Return height of the peak
        double GetHeight(void);
    /// Return intensity of the peak
        double GetIntensity(void);
    /// Return width of the peak
        double GetWidth(void);
    /// Return asymmetry of the peak
        virtual double GetAsymmetry(void);
    /// Estimate peak position for creating a peak
        void	EstimatePeakPosition(float &E00, size_t &ip, const int L, const int R);
    /// Return no of parameters in use
        virtual int		GetNoOfTerms(void);	// Return no of parameters to use
    /// Complete peak-descriptor text
        virtual	void	GetText(wxStringList **TheText);
    /// Initialize peaks
    void Init(void);
    /// Return true if peak intensity is represented by the area
    bool IsIntensityArea(void)
    {   return fpPk_Intensity == (ComponentFlag & fpPk_Intensity);}
    /// Post-processing after creating the peaks
        virtual void PostProcessing(void);
    /** Process the next point (\see Peak1Dim::StepDir) */
        virtual void ProcessPoint(double work);
    /// Prepare peak creation
        virtual bool	Setup(void);
  /* Routines for calculating the natural width and spectrometer width from each other
     based mainly on
        J. J. Olivero and R.L. Longbothum,
        J. Quant. Spectrosc. Radiat. Transfer. Vol 17, page 233 (1977)
        More precise approximation for the Voigt half width
  */
    /// Helper function for calculating the Voigt width
    /** \param  D   function parameter
        \return the R parameter, \see Voigt_Width */
    static double Voigt_R (const double D)
    {
        return 1-0.1821*(1-D*D)+(0.023665*exp(0.6*D)+0.00418*exp(-1.9*D))*sin(PI*D);
    }
    /// Return the Voight width for a peak with Lorentzian/Gaussian width ratio y
    /** \param  y   the ratio of the Lorentzian width to the Gaussian width
        \return width of the resulting Voight function */
    static double Voigt_Width(const double y)
    {
        double  yy = y + sqrtln2;
        return yy * Voigt_R((y-sqrtln2)/yy);
    }

    /// Returns Voigt Y parameter (the ratio of the Lorentzian/Gaussian width
    /** \param  sG  width of the Gaussian
        \param  sL  width of the Lorentzian
        \return the y parameter fro the Voigt function */
    static double Voigt_Y(const double sG, const double sL )
    {
            return 0==sG ? 1E9 : sqrtln2*sL/sG;
    }

    /// Returns Voigt X parameter
    /** \param  E_Minus_E0  the energy deviation from the main energy
        \param  sG          the width of the Gaussian
        \return the X parameter for the Voigt shape */
    double Voigt_X(const double E_Minus_E0, const double sG )
    {
            return 0== sG ? 0 : sqrtln2 * E_Minus_E0/sG;
    }//Voigt_X

protected:
        short int StepDir;  ///< A work variable : direction a calculation step
        double  E0,          ///< Energy
            Y0,         ///< Height/Area
            W0,         ///< Width
            A0,         ///< Asymmetry
            sigma,      ///< Width-related par
            TheArea;    ///< The area
        size_t  I0;         ///< Peak position, in channels
        bool    FinishPeak; ///< True if to finish the peak
};// of class Peak

/// 1-dimensional peak class creation dialog
class PeakDialog : public ComponentDialog
{
public:
    /// Ctor for peak parametrization
        PeakDialog(wxWindow *parent, wxWindowID id, const wxString& title,
                                const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                                long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = "dialogBox");
    wxCheckBox  *IntensityIsArea;   ///< checkbox for selectring intensity mode
};//of class PeakDialog


#endif // ewa_peakh
#endif // oldPeak1
