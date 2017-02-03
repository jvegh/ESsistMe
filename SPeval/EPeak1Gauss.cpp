/*!  @brief Component base for the ESsistMe tool
 * Stuff for implementing "Component Peak Gaussian" type
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <math.h>
#include <algorithm>
#include <string>
#include "EPeak1Gauss.h"
extern bool UNIT_TESTING;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// EPeak1Gauss
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class EPeak1Gauss
  \brief Base class for 1-dimensional spectrum components
        Implements the behaviour of components, independently of the type.
        ComponentBase::ComponentFlag is a bitmap for parameter presence
        ComponentBase::FeatureFlag used differently by the different components

        \see ESpectrumBase

*/
    EPeak1Gauss::
EPeak1Gauss( ESpectrum1Dim *aSpectrum, int aWIdent, const string& aLabel):
         EPeak1Dim(aSpectrum, aWIdent,aLabel)
{
   m_ComponentSubType = opk_Gauss;
}

#if defined(OldGauss1)

    /// Gaussian -like peak class
    class PeakGauss : public Peak1Dim
    {public:
        /// Construct a peak with ID and label, for a spectrum
            PeakGauss(Spectrum1Dim *aSpectrum, const int aWIdent, wxString aLabel);
        /// Copy ctor for Gaussian -like peak
            PeakGauss(const PeakGauss &P) ;
        /// Ctor for Gaussian -like peak
            PeakGauss(void);
        /// Return the area-to-height factor
            virtual double AreaToHeight(void);
        /// Calculate the component
            virtual void Calculate(void);
        /// Calculate peak area
            virtual double CalculateArea(void);
        /// Return a cloned copy of the Gaussian -like peak
            virtual wxParameterSet *Clone(void) const
                    {return new PeakGauss(*this);};
        /// Create component select dialog for the Gaussian -like peak
        virtual ComponentDialog* CreateComponentDialog(SpectrumView *view);
        /// Initialize peak
        void Init(void);
        /// Setup the component
            virtual bool Setup(void);
        /// Sum up counts between limits
            double SumUp(int Low ,int High);
            //virtual Procedure Print(var PrintFile : text; var PrintLine : word);
    };// of class PeakGauss

    /// Gaussian -like peak class creation dialog
    class PeakGaussDialog : public PeakDialog
    {public:
        /// Ctor for the Gaussian parameter select dialog
            PeakGaussDialog(wxWindow *parent, wxWindowID id, const wxString& title,
                                    const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                                    long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = "dialogBox");
    };


#endif // OldGauss
