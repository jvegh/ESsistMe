/*!  @brief Component base for the ESsistMe tool
 * Stuff for implementing "Component peak" type
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <math.h>
#include <algorithm>
#include <string>
#include "EPeak1Dim.h"
extern bool UNIT_TESTING;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// EPeak1dim
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class EPeak1Dim
  \brief Base class for 1-dimensional spectrum components
        Implements the behaviour of components, independently of the type.
        ComponentBase::ComponentFlag is a bitmap for parameter presence
        ComponentBase::FeatureFlag used differently by the different components

        \see ESpectrumBase

*/
/**
    Ctor for Component1.

    \see ESpectrumBase
    \param	aSpectrum	The spectrum the component belongs to
    \param	aWIdent		The individual ID of the component to create
    \param	aLabel		The label string for the component to create
    \param  Root        If the parameter will be root of a spectrum
*/
    EPeak1Dim::
EPeak1Dim( ESpectrum1Dim *aSpectrum, int aWIdent, const string& aLabel ):
         EComponent1Dim(aSpectrum, aWIdent, aLabel)
{
}

#if defined(OldPeak1)
    //: peak1.cpp
    /*  Peak -in  1 dimension type, base stuff
        \author   (c) JÃ¡nos VÃ©gh, MTA ATOMKI, Debrecen, Hungary (veghj@users.sf.net)
        \since    Nov 28, 2004
        $Id: peak1.cpp,v 1.1 2004-07-12 11:08:42+02 veghj Exp veghj $
     */

    #ifdef __GNUG__
        #pragma implementation "peak1.h"
    #endif

    #include "wx/wxprec.h"

    #ifdef __BORLANDC__
    #pragma hdrstop
    #endif

    #ifndef WX_PRECOMP
    #include "wx/wx.h"
    #endif

    #ifdef __WINDOWS__
        #include <strstrea.h>
    #else
        #include <strstream.h>
    #endif

    #include	"../include/params.h"
    #include	"../include/ctypes.h"
    #include	"../include/peak1.h"
    #include	"../include/pkdonsun.h"
    #include	"../include/pkgauss.h"
    #include	"../include/pklorent.h"
    #include	"../include/pkvoigt.h"
    #include	"../include/pkkuciev.h"
    #include	"../include/pkdrude.h"

    wxList	EWAPeakTypes;

    IMPLEMENT_DYNAMIC_CLASS(Peak1Dim,Component1Dim)

    //------------------- Methods of Peak1Dim ------------------------
    /**
        ComponentBase::ComponentFlag is a bitmap for parameter presence.
        The first 3 bit positions (bits 0,1,2) are always set, since
        all peaks must have energy, intensity and width. Because of this,
        these 3 bits are used differently:
        - bit 1 is set if the intensity is represented by area
     */
    /**
        \param  aSpectrum   the spectrum this peak belongs to
        \param  aWIdent     the identifier for this component
        \param  aLabel      the label for this component
    */
    Peak1Dim::Peak1Dim(Spectrum1Dim *aSpectrum, const int aWIdent, wxString aLabel):
    Component1Dim(aSpectrum, aWIdent, false, aLabel)
    {
        Init();
        ComponentBase* R = GetRoot();
        if(R)
            GetStore(R->GetNMin(),R->GetNMax());
    }// of Peak1Dim

    Peak1Dim::Peak1Dim(const Peak1Dim &P) :
    Component1Dim(P)
    {
    }// Peak1Dim

    Peak1Dim::Peak1Dim():
    Component1Dim()
    {
        Init();
    }// of Peak

    void Peak1Dim::Init(void)
    {
        ComponentType = ot_Peak;
        Contributing = true;	//Peaks are contributing components
        ClassName = _("Peak");
        FeatureFlag = 0;
        // Assume using energy, intensity and width terms
    //	ComponentFlag = fpPk_Position | fpPk_Intensity | fpPk_Width;
    }

    int  Peak1Dim::Create(int CommandCode, wxList *aTypes, size_t &cindex)
    {
        Spectrum1Dim *SP = (Spectrum1Dim*) TheSpectrum;
        Component1Dim *OP = SP->GetActiveComponent(); // Get the old (empty) component
        double OldE = GetParameterValue(0); // Save old peak parameters
        double OldY = OldY = GetParameterValue(1);
        if(wxID_OK == Component1Dim::Create(CommandCode, aTypes, cindex))
            {// O.K., a peakcomponent has been selected and succesfully created
                if(SP->GetActiveComponent()->GetType() == ot_Peak)
                        {   OP->SetParameterValue(0,OldE); OP->SetParameterValue(1,OldY);}
                return wxID_OK;
            }
            else return wxID_CANCEL;
    }//Component1Dim::Create

    void   Peak1Dim::GetText(wxStringList **TheText)
    {	char cs[256]; ostrstream outs(cs,sizeof(cs));
        Component1Dim::GetText( TheText );
    //	outs << "This is a peak-type component" << ends;
    //	(*TheText)->Add(cs);
    }
    double Peak1Dim::GetArea()
    {
        if(IsIntensityArea())
            return GetIntensity();// The AREA is the intensity, simple return it
        else
            return GetIntensity()/AreaToHeight();// The HEIGHT represents intensity, so calculate area
    }
    double Peak1Dim::GetEnergy()
        {	return GetParameterValue(0);}

    double Peak1Dim::GetHeight()
    {
        if(IsIntensityArea())
            return GetIntensity()*AreaToHeight();// The AREA is the intensity, calculate it
        else
            return GetIntensity();	// The HEIGHT represents intensity
    }

    double Peak1Dim::GetIntensity()
        {	return GetParameterValue(1);}
    double Peak1Dim::GetWidth()
        {	return GetParameterValue(2);}

    double Peak1Dim::GetAsymmetry(void)
        {	return (ComponentFlag & fpPk_GAsymmetry) ? GetParameterValue(3) : 0;}

    ComponentDialog* Peak1Dim::CreateComponentDialog(SpectrumView *view)
    {
        return new PeakDialog(view->GetFrame(), -1,wxString::Format(_("Select %s terms"),GetClassName().c_str()));
    }

    // virtual function, to set up parameters shown in component dialog
    // Called from CreatePrepared
    void	Peak1Dim::CreateSetParameters(void)
    {
        // Set if the intensity is area
        ((PeakDialog*)CD)->IntensityIsArea->SetValue( fpPk_Intensity == (ComponentFlag & fpPk_Intensity) );
        // Restore the permanent flags
        ComponentFlag |= fpPk_Position | fpPk_Intensity | fpPk_Width;
        for(int w = 0; w<4; w++)
        {
            bool wf = !(0 == (ComponentFlag & (1 << w)));
            CD->CheckBoxes[w]->SetValue(wf);
        }
    }//Peak1Dim::CreateSetParameters

    // virtual function, to read out parameters shown in component dialog
    // Called from CreatePrepared
    void	Peak1Dim::CreateGetParameters(void)
    {
        ComponentFlag = 0; int w;
        for(w = 0; w<4; w++)
            if( CD->CheckBoxes[w]->GetValue())
                ComponentFlag |= 1 << w;
        // for peaks, the position, intensity and width are always present,
        // so this position is used for other purposes
        ComponentFlag &= ~(fpPk_Position | fpPk_Intensity | fpPk_Width);
        if((((PeakDialog*)CD)->IntensityIsArea->GetValue()))  // Read out Area/Height flag
            ComponentFlag |= fpPk_Intensity; // Set if AREA is the measure
    }//Peak1Dim::CreateGetParameters


    // Estimate peak position, between limits L and R,
    // using the 'centre of mass'
    void Peak1Dim::EstimatePeakPosition(float &E00, size_t &ip, const int L, const int R)
    {
        double w =0; double w1 = 0; double w2, w3;
        Spectrum1Dim *SP = (Spectrum1Dim*)GetSpectrum();
        float Ymn = fabs(SP->GetNetC(L)); float Ymx = Ymn; ip = L;
        for(int i = L; i<= R; i++)
        {
            w2 = SP->GetNetC(i); w3 = fabs(w2);
            if(w3 < Ymn) Ymn = w3;
            if(w3 > Ymx) { Ymx = w3; ip = i;}
            w += w2; w1 += GetE(i) * w2;
        }
        E00 = w1/w;     // This is the centre of mass
    }// of Peak1Dim::EstimatePeakPosition

    int Peak1Dim::GetNoOfTerms(void)
    {
        int SaveC = ComponentFlag; ComponentFlag |= 0x7;
        int N = Component1Dim::GetNoOfTerms();
        ComponentFlag = SaveC;
        return N;
    }// Peak1Dim::GetNoOfTerms

    // Process on point of the peak
    void Peak1Dim::ProcessPoint(double work)
    {
        FinishPeak = fabs(work) < TheSpectrum->MinimumCount/10;
        Component1Dim* PRoot = (Component1Dim*) GetRoot();
        if(!FinishPeak)
            FinishPeak = (Index0<(int)PRoot->GetILow()) || (Index0>(int)PRoot->GetIHigh());
        if(FinishPeak)
        {
            if (StepDir<0)
            {// it was only the left wing
                FinishPeak = false; StepDir = +1;
                Index0 = I0;
            }
        }
        else
        {
            ObjectStore[Index0-ObjectBegin] = (float)work;
            if(Index0<(int)ILow) ILow = Index0;
            if(Index0>(int)IHigh) IHigh = Index0;
        }
        Index0 += StepDir;
    }

    void Peak1Dim::Calculate(void)
    {
        Component1Dim::Calculate();	// Clear HasChanged
        Component1Dim *R = (Component1Dim*)GetRoot();
        GetStore(R->GetNMin(), R->GetNMax()); // Reserve storage
        E0 = GetEnergy();
        I0 = GetIndexOfE((float)E0);
        A0 = GetAsymmetry();
        if (I0<R->GetNMin()) I0 = R->GetNMin();
        if (I0>R->GetNMax()) I0 = R->GetNMax();
        W0 = GetWidth();
    /*	if (IsIntensityArea())
            Y0 = GetArea();
        else
    */        Y0 = GetHeight();
        StepDir = -1; FinishPeak = false; ILow = I0; IHigh = I0; Index0 = I0;
    }//of Calculate


    bool Peak1Dim::Setup(void)
    {
        if (!Component1Dim::Setup())
            return false;
        // Initialize the basic peak parameters
        NoOfItems = GetNoOfTerms();
        Parameters = new wxParameterRecord[NoOfItems];
        Spectrum1Dim *SP = (Spectrum1Dim*)GetSpectrum();
        size_t i, ip; double w=0.0,w1=0.0,w2,w3,ww;
        // If the markers are at extreme position, look for the marker positions
        if ((MrkL==NMin) || (MrkR==NMax))
        {	//Find first the half maximum; 3-point rolling average
            w = SP->GetNetC(MrkL)+SP->GetNetC(MrkL+1);
            double Ymn = fabs(w); double Ymx = Ymn; ip = MrkL+1;
            for(i = MrkL+1; i<MrkR; i++)
            {	w += SP->GetNetC(i+1);
                if(fabs(w) < Ymn) Ymn = fabs(w);
                if(fabs(w) > Ymx) { Ymx = fabs(w); ip = i;}
                w -= SP->GetNetC(i-1);
            }
            w = (Ymx-Ymn)/2/3; // now w contains the half height
        }
        // If left marker is at default, set to half width
        if((MrkL==NMin) && (ip>NMin))
        {	//go to the assumed half width, left side
            MrkL = ip;	w1 = w; // start from the maximum
            do
            {	w2 = w1; MrkL--; w1 = fabs(SP->GetNetC(MrkL));
            } while ((w<w1) && (MrkL>NMin));
            if((fabs(w1-w)>fabs(w2-w)) && !(MrkL==NMin)) MrkL++;
        }
        // If rightt marker is at default, set to half width
        if ((MrkR==NMax) && (ip<NMax))
        {	//go to the assumed half width, right side
            MrkR = ip-1;
            do
            {	w2 = w1; MrkR++; w1 = fabs(SP->GetNetC(MrkR));
            } while ((w1>w) && (MrkR<NMax));
            if ((fabs(w1-w)>fabs(w2-w)) && !(MrkR==NMax)) MrkR--;
        }
        // Now the two markers are surely at the assumed half heights
        if( MrkL==MrkR)
        {	//the width will be zero, prevent it
            if(MrkL==NMin) MrkR++; else MrkL--;
            if(MrkR==NMax) MrkL--; else MrkR++;
        }
        // Now determine the initial peak position : calculate centre of mass
        float E00;
        EstimatePeakPosition(E00, ip, MrkL, MrkR);
        w3 = fabs(GetE(MrkR)-GetE(MrkL))/50;
        int dc = SP->AxisE.GetDecimals(); if(dc>0) ++dc;
        InitItem(0,_("Energy"),E00,0//mask
                 ,GetE(MrkL),GetE(MrkR),(float)w3,(float)w3/10,SP->AxisE.GetDigits(),dc);
        // Now set up the width parameter
        w2 = fabs(GetE(MrkR)-GetE(MrkL));
        //  Needs pinpointing, with interpolation
        InitItem(2,_("Width"),w2,pmLogDisplay//mask
                 ,(float)w2/3,(float)w2*3,(float)w2/20,(float)w2/100,SP->AxisE.GetDigits(),dc);
        // Now set up the intensity parameter
        W0 = w2;    // The width parameter might be needed
        w2 = SP->GetNetC(ip);	// Get the max. net intensity
        w3 = SP->GetdC(ip);
        int dd = SP->AxisC.GetDigits();
        int j1 = 0;
        if( IsIntensityArea())
        {   // The intensity is represented with the area
            w1 = AreaToHeight();
            w2 /= w1;
            w3 /= w1;
            j1 = (int)log10(1/w1);
        }
        // Correct for efficiency if needed
        if (SP->GetEffData())
        {
            ww = SP->GetEff(ip);
            if(!(0==ww)){ w3 /= ww; w2 /= ww ;}
        }
        dc = SP->AxisC.GetDecimals(); if(dc>0) ++dc;
        if(j1>0)
            dd +=j1;
        else
            dc -= j1;
        InitItem(1,_("Intensity"),w2,pmLogDisplay//mask
            ,(float)w2/5,(float)w2*5,(float)w3,(float)w3/10,dd,dc);
            // the intensity might be <0

        // Now set up asymmetry if present
        if(ComponentFlag & fpPk_GAsymmetry)
        {
            w1 = w2 = 0;
            for(i = MrkL; i<=ip; i++) w1 += SP->GetNetC(i);
            for(i = ip; i<= MrkR; i++) w2 += SP->GetNetC(i);
            w2 +=w1; w1 = (w1-w2/2)/w2;
            InitItem(3,_("Asymmetry"),w1,0//mask
                     ,-0.999F,+0.999F,0.05F,0.005F,6,4);
        }
        return true;
    }// of Setup

    // Execute the after-peak calculation operations
    void Peak1Dim::PostProcessing(void)
    {
    }//PostProcessing

    PeakDialog::PeakDialog(wxWindow *parent, wxWindowID id, const wxString& title,
                    const wxPoint& pos , const wxSize& size ,
                    long style , const wxString& name ):
    ComponentDialog( parent, id, title, pos, size, style, name)
    {   // Type in parameters with their names
        CheckBoxes[0] = new wxCheckBox(this,-1,_("&Energy"));
        CheckBoxes[1] = new wxCheckBox(this,-1,_("&Intensity"));
        CheckBoxes[2] = new wxCheckBox(this,-1,_("Total &Width"));
        chboxsizer->Add( CheckBoxes[0], 1, wxALL, 5 );
        chboxsizer->Add( CheckBoxes[1], 1, wxALL, 5 );
        chboxsizer->Add( CheckBoxes[2], 1, wxALL, 5 );
        // All parameters are pre-selected; the last one is optional
        for(int i=0; i<3; i++)
        {
            CheckBoxes[i]->SetValue(true);
            CheckBoxes[i]->Enable(false);
        }
        // Now set up the options box
        wxStaticBox *optbox = new wxStaticBox(this, -1, _("Options"));
        optionssizer = new wxStaticBoxSizer( optbox, wxVERTICAL );
        controlinternal->Add(optionssizer, 1, wxEXPAND | wxALL | wxALIGN_TOP, 1);
        IntensityIsArea = new wxCheckBox(this,-1,_("Intensity is &Area"));
        optionssizer->Add( IntensityIsArea, 1, wxALL | wxALIGN_TOP, 5 );
        ResizeDialog();
    }//PeakDialog constructor

    void InitPeaks(void)
    {
        EWAPeakTypes.Append(new PeakGauss());
        EWAPeakTypes.Append(new PeakLorentz());
        EWAPeakTypes.Append(new PeakpVoigt());
        EWAPeakTypes.Append(new PeakVoigt2());
    /*	EWAPeakTypes.Append(new PeakVoigt4());
        EWAPeakTypes.Append(new PeakVoigt6());*/
        EWAPeakTypes.Append(new PeakVoigt());
        EWAPeakTypes.Append(new PeakKuchiev());
        EWAPeakTypes.Append(new PeakKuchievConv());
        EWAPeakTypes.Append(new PeakDoniachSunjic());
    //	EWAPeakTypes.Append(new PeakDoniachSunjicConv());
        EWAPeakTypes.Append(new PeakDrude1());

    //	EWAPeakTypes.Append(new PeakDrude2());
    }

#endif // wxOld
