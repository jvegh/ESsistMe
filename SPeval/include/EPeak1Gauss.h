/*! @brief ComponentBase for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef ESsistMe_Epeak1Gauss_H
#define ESsistMe_EPeak1Gauss_H
#include <string>
#include <iostream>
#include <vector>
#include "EPeak1Dim.h"
#include "ESpectrum1Dim.h"
//#include "MinuitParameter.h"
using namespace std;


/*! @brief Base class for 1-dimensional spectrum components */
class EPeak1Gauss : public EPeak1Dim
{
public:
  EPeak1Gauss (ESpectrum1Dim *aSpectrum, int aWIdent,  const string& aLabel);
protected:
  ESpectrumBase    *m_Spectrum;   ///< Back pointer to the owner spectrum, a copy
};


#endif //  ESsistMe_EComponentBase_H
#if defined(OldGauss1)
//: pkgauss.cpp
/*  Gaussian Peak - in 1 dimension - stuff
    \author   (c) JÃ¡nos VÃ©gh, MTA ATOMKI, Debrecen, Hungary (veghj@users.sf.net)
    \since    Feb 28, 2004
    $Id: pkgauss.cpp,v 1.0 2004-05-03 21:17:47+02 veghj Exp veghj $
 */

#ifdef __GNUG__
        #pragma implementation "pkgauss.h"
#endif
#include "wx/wxprec.h"

#ifdef __BORLANDC__
        #pragma hdrstop
#endif

#ifndef WX_PRECOMP
        #include "wx/wx.h"
#endif

#include		"../include/pkgauss.h"

//------------------- Methods of PeakGauss ------------------------
/**
    \class  PeakGauss

    Peak of form
    \f$
    Y_{i}^{Gauss}=Y_{0}\cdot \exp \left( -\left( \frac{E_{i}-E_{0}}{W_{0}\cdot 0.6005}\right) ^{2}\right)
    \f$
<BR>
where
<BR>\f$ E_{0}\f$ is the position of the peak
<BR>\f$ Y_{0}\f$ is the height of the peak
<BR>\f$ W_{0}\f$ is the width  of the peak

    \param  aSpectrum   the spectrum this peak belongs to
    \param  aWIdent     the identifier for this component
    \param  aLabel      the label for this component
*/
PeakGauss::PeakGauss(Spectrum1Dim *aSpectrum, const int aWIdent, wxString aLabel):
Peak1Dim(aSpectrum, aWIdent, aLabel)
{
    Init();
}// of PeakGauss

PeakGauss::PeakGauss(const PeakGauss &P)
:Peak1Dim(P)
{
}// of PkGauss copy constructor

PeakGauss::PeakGauss():
Peak1Dim()
{
    Init();
}// of PeakGauss


void PeakGauss::Init(void)
{
        ComponentSubType = opk_Gauss;
        ClassName << "|Gauss";
}

double PeakGauss::AreaToHeight(void)
{	return 0.9394373 /*=2*sqrt(ln(2)/Pi)*//W0; }

double PeakGauss::CalculateArea(void)
{	return (ComponentFlag & fpPk_Intensity)
                ? Y0
                : Y0*W0*1.064467;
}

void PeakGauss::Calculate(void)
{	Peak1Dim::Calculate();	// Clear storage
        double b=0.600561 ;
        sigma = 1/W0/b;
        do
        {	double work = GetE(Index0)-E0;
                if( ComponentFlag & fpPk_GAsymmetry )
                        work *= work >0 ? (1+A0) : (1-A0);
                work *= sigma;
                work = (fabs(work)< 3.4) ? exp ( - work*work) * Y0 : 0;
                ProcessPoint(work);
        } while (!FinishPeak);
}// Calculate


ComponentDialog* PeakGauss::CreateComponentDialog(SpectrumView *view)
{
    return new PeakGaussDialog(view->GetFrame(), -1,wxString::Format(_("Select %s terms"),GetClassName()));
}

bool PeakGauss::Setup()
{
    bool b = Peak1Dim::Setup();
        // Now set up asymmetry if present
        if(ComponentFlag & fpPk_GAsymmetry)
        {
        size_t i;
                double w1 = 0; double w2 = 0; size_t ip = (MrkL+MrkR)/2;
        Spectrum1Dim *SP = (Spectrum1Dim*)GetSpectrum();
                for(i = MrkL; i<=ip; i++) w1 += SP->GetNetC(i);
                for(i = ip; i<= MrkR; i++) w2 += SP->GetNetC(i);
                w2 +=w1; w1 = (w1-w2/2)/w2;
                InitItem(3,_("Asymmetry"),w1,0//mask
                                 ,-0.999F,+0.999F,0.05F,0.005F,6,4);
        }
    return b;
}

PeakGaussDialog::PeakGaussDialog(wxWindow *parent, wxWindowID id, const wxString& title,
                                const wxPoint& pos , const wxSize& size ,
                                long style , const wxString& name ):
PeakDialog( parent, id, title, pos, size, style, name)
{
        CheckBoxes[3] = new wxCheckBox(this,-1,_("&Asymmetry"));
    chboxsizer->Add( CheckBoxes[3], 1, wxALL, 5 );
    ResizeDialog();
}//PeakGaussDialog constructor




#endif // oldGauss1
