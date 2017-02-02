/*!  @brief Component base for the ESsistMe tool
 * Stuff for implementing "Component base" type
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <math.h>
#include <algorithm>
#include <string>
#include "EComponent1Dim.h"
extern bool UNIT_TESTING;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// EComponent1dim
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class EComponent1Dim
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
    EComponent1Dim::
EComponent1Dim( ESpectrum1Dim *aSpectrum, int aWIdent, const string& aLabel, bool Root ):
        EComponentBase(aSpectrum, aWIdent, Root, aLabel)
{
        Setup();
}


    bool EComponent1Dim::
Setup(void)
{
        if(!EComponentBase::Setup()) return false;
/*        Component1Dim *R = (Component1Dim *)NULL;
        if(!IsRoot()  // Other than the root
            && TheSpectrum && (R=(Component1Dim*)TheSpectrum->GetRoot()) )
        {
            for (int i=0; i<gsMarker+1; i++)
                GraphStyle[i] = R->GraphStyle[i];
            GetECurrent = R->GetECurrent;
            DrawAllComponents = false;
        }
        else    // This is the root
        {
            for (int i=0; i<gsMarker+1; i++)
                GraphStyle[i] = EWADefault.GraphStyle[i];
            if( IsOnBindingScale() == xboolean::True)
                SetAxisModeE( (amAxisMode)(GetAxisModeE() | am_Reversed));
            SetETransformed(false);
            DrawAllComponents = true;
        }
*/        if(m_Spectrum)
            GetStorage(0,m_Spectrum->NoOfDataPoints_Get()-1);

        return true;
}// EComponent1Dim::Setup


#if defined(wxOld)
    //: comp1.cpp
    /*  Component types in 1 dimension
        \author   (c) JÃ¡nos VÃ©gh, MTA ATOMKI, Debrecen, Hungary (veghj@users.sf.net)
        \since    Dec 09, 2004
        $Id: comp1.cpp,v 1.1 2004-08-23 14:35:47+02 veghj Exp veghj $
     */

    #ifdef __GNUG__
        #pragma implementation "comp1.h"
    #endif

    #include "wx/wxprec.h"

    #ifdef __BORLANDC__
        #pragma hdrstop
    #endif

    #ifndef WX_PRECOMP
        #include "wx/wx.h"
    #endif

    #include  "../include/menu_ids.h"

    #include    "../include/spectr1.h"
    #include    "../include/spwind1.h"
    #include    "../include/defaults.h"
    #include    "../include/specproc.h"
    extern  Defaults EWADefault;
    extern  EWABatchProcessor BatchProcessor;
    extern wxSpectrumProcessor SpectrumProcessor;
    //extern    void    SetDisplayMarkerSub(SpectrumView *w);
    //extern    void    SetDisplayZoomSub(SpectrumView *w, float E1, float E2, float C1, float C2);

    /*
     * 1-dimensional component staff
     */
    IMPLEMENT_DYNAMIC_CLASS(Component1Dim,ComponentBase)


    /// Create a component for aSpectrum, with ident aWIdent and label aLabel
    Component1Dim::Component1Dim(Spectrum1Dim *aSpectrum, const int aWIdent, const bool Root, wxString aLabel)
    :       ComponentBase( aSpectrum, aWIdent, Root, aLabel)
    {   ObjectStore = NULL; ObjectSize = ObjectBegin = 0;
        Setup();

        if(!IsRoot()  // Other than the root
            && TheSpectrum && TheSpectrum->GetRoot() )
        {
            Component1Dim *R = (Component1Dim*)TheSpectrum->GetRoot();
            for (int i=0; i<gsMarker+1; i++)
                GraphStyle[i] = R->GraphStyle[i];
            XEnergy = R->XEnergy;
            AxisModeC = R->AxisModeC;
            VCMin = R->VCMin; VCStep = R->VCStep; VCMax = R->VCMax;
            CZoom = R->CZoom,CZoomBegin = R->CZoomBegin;
            GetECurrent = R->GetECurrent;
            DrawAllComponents = false;
        }
        else    // This is the root
        {   XEnergy = -1.0F;
            for (int i=0; i<gsMarker+1; i++)
                GraphStyle[i] = EWADefault.GraphStyle[i];
            AxisModeC = EWADefault.AxisModeC;
            CZoom = 1; CZoomBegin = 0;
            if( IsOnBindingScale())
                SetAxisModeE( (amAxisMode)(GetAxisModeE() | am_Reversed));
            SetETransformed(false);
            DrawAllComponents = true;
        }
      MrkL = NMin; MrkR = NMax;
    }//Component1Dim::Component1Dim

    /// Default constructor for the 1-dimensional component
    Component1Dim::Component1Dim(void)
    : ComponentBase()
    {   ObjectStore = NULL;
        ObjectSize = 0;
        DrawAllComponents = false;
    }//Component1Dim::Component1Dim

    /// Copy constructor for the 1-dimensional component
    Component1Dim::Component1Dim(const Component1Dim &T)
    : ComponentBase(T)
    {
        DrawAllComponents = T.DrawAllComponents;
        for( int i=0; i<gsMarker+1; i++)
            GraphStyle[i] = T.GraphStyle[i];
        GetECurrent = T.GetECurrent;
    //    SetETransformed(IsOnBindingScale() == xboolean::True);
    }//Component1Dim::Component1Dim

    /// Destructor for the 1-dimension components
    Component1Dim::~Component1Dim()
    {
        // Must not subtract, leads to error for clones
        FreeStore();
    }//Component1Dim::~Component1Dim



    /// Changes HasChanged in all slaves, linked to this master
    Component1Dim *ThisChanged;
    int ChangedIndex;

    /** Create a component of any type
        \param  CommandCode code for the current command
        \param  aTypes  list of the component types
        \param  cindex  index of the selected component in the list
        \return */
    int  Component1Dim::Create(int CommandCode, wxList *aTypes, size_t &cindex)
    {
        if(wxID_OK == ComponentBase::Create(CommandCode, aTypes, cindex))
            {// O.K., a component has been selected and succesfully created
                Spectrum1Dim *SP = (Spectrum1Dim*) TheSpectrum;
                Component1Dim *OP = SP->GetActiveComponent(); // Get the old (empty) component
                OP->DrawAllComponents = OP->IsContributing() ? ((Component1Dim*)OP->GetRoot())->DrawAllComponents : false;
                return wxID_OK;
            }
            else return wxID_CANCEL;
    }//Component1Dim::Create



    // Calculate the area of the component in ObjStore, works also for non-equidistant data
    double Component1Dim::CalculateArea(void)
    {
        size_t Index = ILow; double Sum = 0;
        float LastE = GetE(Index); float LastdE = 0;
        do
        {
            float w = ObjectStore[Index++];
            float ThisE = GetE(Index); float ThisdE = ThisE-LastE;
            Sum += w*(ThisdE+LastdE);
            LastE = ThisE; LastdE = ThisdE;
        }
        while(Index<IHigh);
        Sum += ObjectStore[Index]*(LastdE);
        return Sum/2; // The half interval should be taken
    }

    double  Component1Dim::CalculatePartMerit(int &i, int j)
    {// Do the merit calculation for points between i and j
        double LastRes = 0; Spectrum1Dim *SP = (Spectrum1Dim*)TheSpectrum;
        while(i<j)
        {
            double ThisRes = SP->GetNetC(i)/SP->GetdC(i);
            AC1 += ThisRes * LastRes;
            Merit += ThisRes*ThisRes;
            LastRes = ThisRes;
            i++; //SP->NoOfFittedPoints++;
        }
        return Merit;
    }

    // Calculate the merit value for the component
    double Component1Dim::CalculateMerit(void)
    {
        Spectrum1Dim *SP = (Spectrum1Dim*)TheSpectrum;
        Merit = 0; AC1 = 0; //SP->NoOfFittedPoints = 0;
        if(SP->GetFitData() && (ot_Nothing != GetType()))
        {   // Calculate sum of squares and autocorrelation terms
            int i = NMin; int j; if(-1==i) i = 0;   // Correct for measurement
            wxNode *node = SP->Excluded->First();
            while(node)
            {   wxPoint *P = (wxPoint*) node->Data();
                CalculatePartMerit(i, P->x);
                i = P->y +1; node = node->Next();
            }// Now all exluded regions processed, finish it
            j = NMax+1;
            CalculatePartMerit(i,j);
    // Now the sum calculated
            if(Merit>0) AC1 /= Merit; else AC1 = 1;
            size_t Total,Active;
            GetTotalNoOfParameters(Total,Active);
            Active = GetNoOfFittedPoints() - Active; if(Active<=1) Active = 1;
            Merit /= Active;
        }
        return Merit;
    }// of CalculateMerit

    /// Do the merit calculation for points between i and j
    /**
        \param  i   (included) lower limit of calculation
        \param  j   (not included) higher limit of calculation
    */
    double  Component1Dim::CalculatePartSum(int i, int j)
    {
        double LastRes = 0; double Sum = 0; Spectrum1Dim *SP = (Spectrum1Dim*)TheSpectrum;
        while(i<j)
        {   // Add new term to the sums
            double ThisRes = SP->GetNetC(i)/SP->GetdC(i);
            AC1 += ThisRes * LastRes;   // Collect autocorrelation sum
            Sum += ThisRes*ThisRes;     // Collect merit sum
    //        SP->NoOfFittedPoints++;     // Count summed points
            LastRes = ThisRes;
            i++;
        }
        return Sum;
    }//Component1Dim::CalculatePartSum

    /** Return sum of squares for the component
        zero is returned if no fitted data or no real component.
        Excluded data are accounted
    */
    double Component1Dim::GetSumOfSquares(void)
    {
        Spectrum1Dim *SP = (Spectrum1Dim*)TheSpectrum;
        double Sum = 0.0; AC1 = 0; //SP->NoOfFittedPoints = 0;    // Initialize calculation
        if(SP->GetFitData() && /*(ot_Nothing != GetType())*/(IsContributing() || (this == SP->GetRoot())))
        {   // we have fitted data and a real component
            int i = NMin; int j; if (-1==i) i = 0;  // Correct for measurement
            wxNode *node = SP->Excluded->First();
            while(node)
            {   // we have more exclusions
                wxPoint *P = (wxPoint*) node->Data();
                Sum += CalculatePartSum(i, P->x);
                i = P->y+1; node = node->Next();
            }// Now all exluded regions processed, finish it
            j = NMax+1;
            Sum += CalculatePartSum(i,j);
        }
        return Sum;
    }//Component1Dim::GetSumOfSquares


    void Component1Dim::PreCreate(SpectrumBase *aSpectrum)
    {
        ComponentBase::PreCreate(aSpectrum);
        Component1Dim*  SC = (Component1Dim*)(aSpectrum->GetActiveComponent());
    //  for (int i=0; i<gsMarker+1; i++)
    //        GraphStyle[i] = SC->GraphStyle[i];
        VCMax = SC->VCMax;  VCMin = SC->VCMin; VCStep = SC->VCStep;
        CZoom = SC->CZoom; CZoomBegin = SC->CZoomBegin;
        MrkL = SC->MrkL;    MrkR = SC->MrkR;
        DrawAllComponents = SC->DrawAllComponents;
        SetETransformed(false);
    }// Component1Dim::PreCreate


    float   ENative(Component1Dim *C, int I) { return C->GetSpectrum()->GetRoot()->GetE(I);}
    float   ETransformed(Component1Dim *C, int I) { return C->GetXEnergy() - C->GetSpectrum()->GetRoot()->GetE(I);}

    void    Component1Dim::SetETransformed(bool Tr)
    { GetECurrent = Tr ? ETransformed : ENative ;}

    // Return component value at point i
    float Component1Dim::GetComp(size_t i)
    {   if(Parameters)
            {   if(HasChanged)
                    { PreCalculate(); Calculate(); PostCalculate();}
                if(ObjectStore && (ObjectBegin<=i) && (i<ObjectBegin+ObjectSize)
                    &&(ILow<=i) && (i<=IHigh) )
                    return ObjectStore[i-ObjectBegin];
                else return 0.0F;
            }
        else return 0.0F;
    }//Component1Dim::GetComp

         /*     CalculateRawParams(PkMax,dPkMax,PkEngy,dPkEngy,pkArea,dPkArea,pkWidth,dPkWidth,Asymm);
              str(PkMax:CDigits:CDecimals,s1);
              str(dPkMax:CDigits:CDecimals,s2);
              TheText^.Insert(NewStr('Raw parameters'));
              TheText^.Insert(NewStr('Max='+s1+'+/-'+s2+' '+sc));
              str(PkArea:CDigits:CDecimals,s1);
              str(dPkArea:CDigits:CDecimals,s2);
              TheText^.Insert(NewStr('Area='+s1+'+/-'+s2+' '+sc+'*'+se));
              str(PkEngy:EDigits:EDecimals,s1);
              str(dPkEngy:EDigits:EDecimals+1,s2);
              TheText^.Insert(NewStr('Center='+s1+'+/-'+s2+' '+se));
              atp := EDecimals; if atp >1 then Inc(atp);
              str(PkWidth:EDigits:atp,s1);
              str(dPkWidth:EDigits:atp,s2);
              TheText^.Insert(NewStr('FWHM='+s1+'+/-'+s2+' '+se));
              str(Asymm:6:4,s2);
              TheText^.Insert(NewStr('Asymmetry='+s2));
         */

    // Calculate no of parameters according to ComponentFlag
    int Component1Dim::GetNoOfTerms(void)
    {   int w = ComponentFlag; int CompNo = 0;
        while(!(0 ==w))
        {   if( w & 1) CompNo++;
            w >>= 1;
        }
        return CompNo;
    }//Component1Dim::GetNoOfTerms

    void    Component1Dim::GetText(wxStringList **T)
    {
        size_t atp,aap;
        wxString a,se,sc;
        if(IsRoot())
            TheSpectrum->GetText(T);
        else
        {   ComponentBase::GetText( T );
            Spectrum1Dim *sp = (Spectrum1Dim*) TheSpectrum;
            se = sp->AxisE.GetScaleUnit();
            sc = sp->AxisC.GetScaleUnit();
            if(!Parameters || (ComponentType==ot_Peak))
                { // Write out the 'raw' parameters
    //              if (ot_Peak==ObjType) SubtractFromFit();
    //              if (ot_Peak==ObjType) AddToFit();
                }
            if(IsOnBindingScale() == xboolean::True)
                (*T)->Add(_("BINDING energy scale is used"));
            if (GetAxisModeE() & am_Reversed)
                (*T)->Add(_("E axis reversed"));
            if (GetAxisModeC() & am_Reversed)
                (*T)->Add(_("C axis reversed"));
            if(sp->Components->GetCount()>1) // we have components
                {
                    sp->GetTotalNoOfParameters(atp,aap);
                    (*T)->Add(wxString::Format(_("Normalized chisq = %s for %i active parameters"),GetFormatted(Merit,2,7),aap));
                    (*T)->Add(wxString::Format(_("1st order autocorrelation = %s"), GetFormatted(AC1,4,6)));
                }
            //display component display limits
            (*T)->Add(wxString::Format(_("Energy from %s %s to %s %s"),
                        sp->AxisE.GetFormatted(VEMin), se, sp->AxisE.GetFormatted(VEMax), se));

            (*T)->Add(wxString::Format(_("Intensity from %s %s to %s %s"),
                        sp->AxisC.GetFormatted(VCMin), sc, sp->AxisC.GetFormatted(VCMax), sc));
            (*T)->Add(wxString::Format(_("Displayed between %s %s [%s] and %s %s [%s]"),
                        sp->AxisE.GetFormatted(GetE(NMin)), se, GetFormatted(NMin,0,5,TRUE),
                        sp->AxisE.GetFormatted(GetE(NMax)), se, GetFormatted(NMax,0,5,TRUE)));
            (*T)->Add(wxString::Format(_("Markers between %s %s [%s] and %s %s [%s]"),
                        sp->AxisE.GetFormatted(GetE(MrkL)), se, GetFormatted(MrkL,0,5,true),
                        sp->AxisE.GetFormatted(GetE(MrkR)), se, GetFormatted(MrkR,0,5,true)));
        }
    }//Component1Dim::GetText


    // Return component value at point i
    void Component1Dim::SetComp(size_t i, float C)
    {   if((ObjectBegin<=i) && (i<ObjectBegin+ObjectSize))
        {   if(i<ILow) ILow = i;
            if(i>IHigh) IHigh = i;
            ObjectStore[i-ObjectBegin] = C;
        }
    }//Component1Dim::SetComp

      void Component1Dim::SetupELimits(float &XMn,float &XMx)
    {
        if (GetAxisModeE() & am_WithError)
        {   XMn = (3*GetE(NMin)-GetE(NMin+1))/2;
            XMx = (3*GetE(NMax)-GetE(NMax-1))/2;
        }
        else { XMn = GetE(NMin); XMx = GetE(NMax);}
    }//Component1Dim::SetUpELimits

    void Component1Dim::SetupVLimits(float &XMn,float &YMn,float &XMx,float &YMx)
    {   Spectrum1Dim *sp = (Spectrum1Dim*) TheSpectrum;
        YMn = sp->GetMinCBetween(NMin,NMax,!(0==(GetAxisModeC() & am_WithError)));
        YMx = sp->GetMaxCBetween(NMin,NMax,!(0==(GetAxisModeC() & am_WithError)));
        if((YMn<=0) && ((GetAxisModeC() & am_Log)==am_Log)) YMn = sp->MinimumCount;
        if(YMx<=YMn) YMx = YMn + 1;
        if((YMn<0) && ((GetAxisModeC() & am_Log)==am_Sqrt)) YMn = 0.0F;
        SetupELimits(XMn,XMx);
    }//Component1Dim::SetUpVLimits

    void Component1Dim::SetVBox(float &XMin,float &YMin,float &XMax,float &YMax)
    {   Scale(XMin,XMax,VEMin,VEStep,VEMax,GetAxisModeE());
        Scale(YMin,YMax,VCMin,VCStep,VCMax,GetAxisModeC());
    }//Component1Dim::SetVBox

    /** Subtract this component from the fit
        \param  aS   the spectrum to be subtracted from */
    void Component1Dim::SubtractFromFit(SpectrumBase *aS)
    {
        Spectrum1Dim *S = (Spectrum1Dim*)aS;
        if(!S)
            S = (Spectrum1Dim *)GetSpectrum();
        GetComp(GetILow()); size_t i;// Dummy GetComp, for case when ILow/IHigh changed
        for(i = GetILow(); i<= GetIHigh(); i++)
            S->SetFit(i,S->GetFit(i) - GetComp(i));

        wxString A;
        for(size_t j = 0; j< GetCount(); j++)
            A += GetFormattedValue(j) + " ";
        wxLogTrace("Computing", "Subtract %s(%s) from %s", GetLabel(), A, S->GetLabel());

        for(i = 0; i<GetAffectedListCount(); i++)
        {
            long ID = GetAffectedListItem(i);
            Component1Dim* CP = (Component1Dim*)SpectrumProcessor.FindComponentByUniqueID(ID);
            wxCHECK2_MSG(CP, break, "Affected list member not found");
            CP->SetHasChanged(true);
            wxString A;
            for(size_t j = 0; j< GetCount(); j++)
                A += CP->GetFormattedValue(j) + " ";
            wxLogTrace("Computing", "Subtract affected %s(%s) from %s", CP->GetLabel(), A, S->GetLabel());
            CP->SubtractFromFit(CP->GetSpectrum()); // Subtract affected components
        }
    }

    /** Add this component to the fit
        \param  aS   the spectrum to be added to */
    void    Component1Dim::AddToFit(SpectrumBase *aS)
    {
        Spectrum1Dim *S = (Spectrum1Dim*)aS;
        if(!S)
            S = (Spectrum1Dim *)GetSpectrum();
        GetComp(GetILow()); size_t i;// Dummy GetComp, for case when ILow/IHigh changed
        for(i = GetILow(); i<= GetIHigh(); i++)
            S->SetFit(i,S->GetFit(i)+GetComp(i));

        wxString A;
        for(size_t j = 0; j< GetCount(); j++)
            A += GetFormattedValue(j) + " ";
        wxLogTrace("Computing", "Add %s(%s) To %s", GetLabel(), A, S->GetLabel());

        for(i = 0; i<GetAffectedListCount(); i++)
        {
            long ID = GetAffectedListItem(i);
            Component1Dim* CP = (Component1Dim*)SpectrumProcessor.FindComponentByUniqueID(ID);
            wxCHECK2_MSG(CP, return, "Component to add not found");
            CP->SetHasChanged(true);
            wxString A;
            for(size_t j = 0; j< GetCount(); j++)
                A += CP->GetFormattedValue(j) + " ";
            wxLogTrace("Computing", "Add affected %s(%s) To %s", CP->GetLabel(), A, S->GetLabel());
            CP->AddToFit(CP->GetSpectrum());    // Add the affected components
        }
    }//Component1Dim::AddToFit

    /**
        \param  Low     Low bound of the region
        \param  High    High bound of the region
        \return the summed up net counts
    */
    double Component1Dim::GetNetCountsBetween(const int Low, const int High)
    {
            double w;	int i;
            double Sum = 0;
            Spectrum1Dim* SP = (Spectrum1Dim*)GetSpectrum();
            for(i = Low; i<= High; i++)
            {	w = SP->GetNetC(i);
                    if(SP->GetEffData())
                    {	double ww = SP->GetEff(i);
                            if(!(0==ww)) w /= ww;
                    }
                    Sum = Sum + w;
            }
            return Sum;
    }


#endif // wxOld
