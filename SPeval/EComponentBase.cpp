/*!  @brief Component base for the ESsistMe tool
 * Stuff for implementing "Component base" type
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <math.h>
#include <algorithm>
#include <string>
#include "EComponentBase.h"
extern bool UNIT_TESTING;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// EComponentBase
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class EComponentBase
  \brief Represents 2-dim component base
        Implements the behaviour of components, independently of the type.
        ComponentBase::ComponentFlag is a bitmap for parameter presence
        ComponentBase::FeatureFlag used differently by the different components

        \see ESpectrumBase

*/
/**
    Ctor for ComponentBase.

    \see ESpectrumBase
    \param	aSpectrum	The spectrum the component belongs to
    \param	aWIdent		The individual ID of the component to create
    \param	aLabel		The label string for the component to create
    \param  Root        If the parameter will be root of a spectrum
*/
    EComponentBase::
EComponentBase(ESpectrumBase *aSpectrum, int aWIdent, bool Root, const string& aLabel) : MinuitParameterSet(aLabel.c_str())
{
        m_Spectrum = aSpectrum;
}

    // sets up some component features;
    // called upon creating a new component
bool EComponentBase::Setup()
{
        HasChanged = true;
        return true;
}//EComponentBase::Setup

// Reserve storage place for component data
void    EComponentBase::GetStorage(int iFrom, int iTo)
{
    ObjectBegin = iFrom;
    int ObjectSize = iTo-iFrom +1;
    ObjectStore.reserve(iTo-iFrom +1);  // Reserve for the number of elements
    std::fill(ObjectStore.begin(),ObjectStore.end(),0);
//    memset(ObjectStore.data(),0,sizeof(ObjectStore[0])*ObjectSize);
}

#define Old_EComponentBase_cpp
#ifndef Old_EComponentBase_cpp

    #include    "wx/docview.h"

    #include    "../include/specproc.h"
    #include    "../include/ewaframe.h"
    #include    "../include/defaults.h"

    extern  void LogMessage(wxString S, bool AddEOL = true);

    #ifndef __USE_GNU
        extern double exp10(double arg);
    #endif	// __USE_GNU
    extern EWABatchProcessor BatchProcessor;
    extern wxSpectrumProcessor SpectrumProcessor;
    extern EWAFrame* MyEWAFrame;
    extern Defaults EWADefault;
    extern void SaveDataDirectory(wxString& s);
    extern int wxAdjustComponent(ComponentBase *comp, SpectrumView *view, const wxString caption, wxWindow *parent);
    extern void	SetDisplayMarkerSub(SpectrumView *w, double LM, double RM);

    IMPLEMENT_DYNAMIC_CLASS(ComponentBase, wxParameterSet)

    ComponentBase::ComponentBase(SpectrumBase *aSpectrum, int aWIdent, bool Root, wxString aLabel)
    :wxParameterSet(0, aLabel)
    {   //ROIList : PCollection;    { of TROIs }
        //ActiveROI : word;         // The currently active ROI
  ///      TheSpectrum = aSpectrum;
        Init(Root);        // Initialize to default
        if(aWIdent<0)
        {   // It is the default, a unique ID was given in Init()
    //        ComponentID = aWIdent;
        }
        else
        {   // A definite ID was given, verify if correct
            ComponentBase* C = SpectrumProcessor.FindComponentByUniqueID(aWIdent);
            if(C)
            {   // There exists a component with such an ID
            }

        }
        if(IsRoot())
        {
            NMin = 0; NMax = TheSpectrum->GetNoOfEPoints()-1;
            Label = ("Root");
            OnBindingScale = xboolean::Unknown;
        }
        else
        {   // Other than root
            ComponentBase *R;
            if(TheSpectrum && (R = TheSpectrum->GetRoot()))
            {
                // Copy settings from root
                NMin = R->NMin; NMax = R->NMax;
                EZoom = R->EZoom; EZoomBegin = R->EZoomBegin;
                VEMin = R->VEMin;VEStep = R->VEStep;VEMax = R->VEMax;
                AxisModeC = R->AxisModeC; AxisModeE = R->AxisModeE;
                VCMin = R->VCMin; VCStep = R->VCStep; VCMax = R->VCMax;
                CZoom = R->CZoom; CZoomBegin = R->CZoomBegin;
                XEnergy = R->XEnergy;
    //            ComponentMode = R->ComponentMode;
                OnBindingScale = R->OnBindingScale;
            }
        }
        ILow = NMin; IHigh = NMax;
    }//ComponentBase::ComponentBase


    /** Default Ctor for ComponentBase.
        Used mainly when creating prototype components */
    ComponentBase::ComponentBase(void)
    :wxParameterSet()
    {
        TheSpectrum = (SpectrumBase*)NULL;
        Init(false);        // Initialize to default
    }


    /** Used mainly when creating prototype components */
    void ComponentBase::Init(bool Root)
    {
        m_Root = Root;                     // The roots are set upon spectrum creating
        Label = _("None");
        ClassName = _("Nothing");
        Contributing    = false;            // By default the components do not contribute
        AlphaMode = false;                  // The default is graphic mode
        UniqueID = SpectrumProcessor.MakeUniqueID();
    //    Parameters = (wxParameterRecord*)NULL;
    //    ComponentMode = 0;
        ComponentFlag = 0;
        FeatureFlag = 0;
        EZoom = 1; EZoomBegin = 0;
        CZoom = 1; CZoomBegin = 0;
        AxisModeC = EWADefault.AxisModeC; AxisModeE = EWADefault.AxisModeE;
        ComponentType = ot_Nothing; ComponentSubType = (ot_ComponentSubTypes)0;
        Merit = 0; AC1 = 0;
    }

    /**	Copy ctor for ComponentBase.
        \param	C	the model for copying */
    ComponentBase::ComponentBase(const ComponentBase &C)
    :wxParameterSet(C)
    {
        //ROIList : PCollection;    { of TROIs }
        //ActiveROI : word;         // The currently active ROI
        EZoom = C.EZoom; EZoomBegin = C.EZoomBegin;
        CZoom = C.CZoom; CZoomBegin = C.CZoomBegin;
        VEMin = C.VEMin; VEStep = C.VEStep; VEMax = C.VEMax;
        VCMax = C.VCMax;  VCMin = C.VCMin; VCStep = C.VCStep;
    //    DrawAllComponents = C.DrawAllComponents;    // If to draw all components in root window

        Contributing = C.Contributing; AlphaMode = C.AlphaMode;
        UniqueID = C.UniqueID;
        ComponentType = C.ComponentType;ClassName = C.ClassName;
        ComponentSubType = C.ComponentSubType;
        OnBindingScale = C.OnBindingScale;
        ComponentType = C.ComponentType;
        AxisModeE = C.AxisModeE;    AxisModeC = C.AxisModeC;
        Index0 = C.Index0;                //  Just a work variable
        Merit = C.Merit; AC1 = C.AC1;
        XEnergy = C.XEnergy;    // Copy excitation energy
        TheSpectrum = C.TheSpectrum;    //ComponentMode = C.ComponentMode;
        NMin = C.NMin; NMax = C.NMax;
        ILow = C.ILow; IHigh = C.IHigh;
        ComponentFlag = C.ComponentFlag;    // Contains 'or'ed fbgx_xx values
        FeatureFlag = C.FeatureFlag;        // Contains features like convolved, struct, etc
        MrkL = C.MrkL; MrkR = C.MrkR;   // Copy markers
        if(C.ObjectSize && C.ObjectStore)
        {// we have stored values, copy them
            ObjectSize = C.ObjectSize;
            ObjectStore = new float[ObjectSize];    // object storage if necessary
            ObjectBegin = C.ObjectBegin;        // 1st channel of the stored object
            memcpy(ObjectStore, C.ObjectStore, ObjectSize*sizeof(ObjectStore[0]));
        }
        else
        {   ObjectStore = NULL; ObjectSize = 0; ObjectBegin = 0;  }
        CD = C.CD;      // Copy component dialog
    }//ComponentBase::ComponentBase

    //Destruct component Base
    ComponentBase::~ComponentBase()
    {
    //  Dispose(ROIList,Done);
    //  Markers.Done;
    }// of ComponentBase::~ComponentBase

    /// Update some settings of a cloned component from the replaced component C
    void ComponentBase::UpdateClonedCopy(const ComponentBase &C)
    {
        SetRoot(false); // Be sure it is not root
        wxString OldS = '|'; wxString NewS = '_';   // Replace command word separator temporarily
        wxString S = GetClassName(); S.Replace(OldS,NewS,true);
        SetLabel(GetSpectrum()->GetUniqueName(S,GetUniqueID()));
        // Now copy some settings since OP cloned with default settings
        XEnergy = C.XEnergy;
        OnBindingScale = C.OnBindingScale;
        AxisModeE = C.AxisModeE;
        AxisModeC = C.AxisModeC;
        MrkL = C.MrkL;
        MrkR = C.MrkR;
    //  OP->ComponentFlag = SC->ComponentFlag; // Do not
    //  OP->FeatureFlag = SC->FeatureFlag;    // Do not
    //?    int     ComponentMode;          ///< The component mode flag word
    /*?    size_t  NMin,                   ///< Lowest sequence# of points
                NMax;                   ///< Highest sequence# of points
        size_t  ILow,                   ///< to calculate the component from this
                IHigh;                  ///< to calculate the component up to this
                */
    }


    /** Link this parameter set to parameter set P
        \param  ID   the unique ID of the master parameter set to link to
    */
    void ComponentBase::LinkTo(long ID)
    {
        ComponentBase *P = SpectrumProcessor.FindComponentByUniqueID(ID);
        wxASSERT_MSG(P, "No master component");
        wxASSERT_MSG(P!=this, "Must not link to itself");
        wxASSERT_MSG(!AttachmentList->Find((wxObject *)ID), "Parameter set linked already to that");
        wxCHECK2_MSG(P, return, "Component not found");

        AttachmentList->Append((wxObject*)ID); SetHasChanged();
        P->AffectedList->Append((wxObject*)this->GetUniqueID());
    }

    /** return true if this parameter set is linked to parameter set P
        \param  ID   the unique ID of the master parameter set to check the link to
    */
    bool ComponentBase::IsLinkedTo(long ID)
    {
        return (wxNode*) NULL != AttachmentList->Find((wxObject*)ID);
    }


    /** UnLink this parameter set from the another one
        \param  ID   the unique ID of the master parameter set to unlink from
    */
    void ComponentBase::UnLinkFrom(long ID)
    {
        wxNode *At = (wxNode *)NULL; wxNode *Af = (wxNode *)NULL;
    //    wxASSERT_MSG(P, "No master component");
        wxASSERT_MSG(ID!=this->GetUniqueID(), "Must not unlink from itself");
        wxASSERT_MSG(At = AttachmentList->Find((wxObject*)ID), "Parameter set no linked to that");
        ComponentBase *P = SpectrumProcessor.FindComponentByUniqueID(ID);
        wxASSERT_MSG(Af = P->AffectedList->Find((wxObject*)ID), "Parameter set not affected by that");
        P->AffectedList->DeleteNode(Af);
        SetHasChanged();
        AttachmentList->DeleteNode(At);
    }


    /// Adjust this parameter
    int ComponentBase::Adjust(SpectrumView *view)
    {
        int ret = wxAdjustComponent(this,view);
        return ret;
    }// ComponentBase::Adjust

    /// Return 'Counts' axis
    AxisRecord *ComponentBase::GetAxisC(void)
    {
        return &(GetSpectrum()->AxisC);
    }

    /// Return 'Energy' axis
    AxisRecord *ComponentBase::GetAxisE(void)
    { return &(GetSpectrum()->AxisE);}

    /**  Return fitted points between i and j
        \param	i	The lower limiting point
        \param	j	The higher limiting point
        \return	result
    */
    size_t ComponentBase::CalculateFittedPointsPart(const size_t i, const size_t j)
    {
        int N = i>=j ? 0 : j-i+1;
        return N;
    }

    /** Create a component of any type
        \param  CommandCode code for the current command
        \param	aTypes	the list of the component subtypes to select from
        \param	cindex	the index of the selected item in the list
        \return	wxID_OK if created or wxID_CANCEL if not
    */
    int  ComponentBase::Create(int CommandCode, wxList *aTypes, size_t &cindex)
    {
        wxCHECK2_MSG( aTypes->GetCount(), return wxID_CANCEL, _("No items in the list"));
        ComponentBase *OP = (ComponentBase*) aTypes->Item(0)->Data();
        wxString s1 = OP->GetClassName().BeforeFirst('|');  // Save the main type
        wxLogTrace("Creating", "Component create %s", OP->GetLabel());
        int SelectType;
        wxBusyCursor wait;  // Show hourglass during processing
        wxString sc;
        if ( (ComponentType==ot_Nothing) || (ComponentType==OP->ComponentType) )
        {   //The component is empty or the same type
            if(!IsRoot())
            {   wxString *choices = new wxString[aTypes->GetCount()];
                for(size_t i=0; i<aTypes->GetCount(); i++)
                {
                    sc = ((ComponentBase*)aTypes->Item(i)->Data())->GetClassName();
                    choices[i] = sc.AfterFirst('|');    //Remove leading "Background/Peak, etc"
                }
                if (BatchProcessor.GetCurrentLevel())
                {   // Get component type from a batch file
                    // Till now the 'COMP Change (Peak | Background | ...) has been read
                    wxString s = BatchProcessor.GetNextToken();
                    for (cindex = 0; cindex<aTypes->GetCount(); cindex++)
                    {   if(s.IsSameAs( choices[cindex])) break;  }
                    if(cindex<aTypes->GetCount())
                        SelectType = wxID_OK;
                    else
                    {   SelectType = wxID_CANCEL; BatchProcessor.Error(BE_WrongParameter); }
                }
                else // Controlled from the keyboard
                {
                    sc = wxString::Format("Select '%s' subtype", s1);
                    int icindex = wxGetSingleChoiceIndex(_("Valid subtypes to choose from"), sc, aTypes->GetCount(), choices, NULL);
                    cindex = icindex;
                    SelectType = icindex<0 ? wxID_CANCEL : wxID_OK;
                }
                delete [] choices;
                if(wxID_CANCEL == SelectType)
                    return wxID_CANCEL;
                ComponentBase *OP = (ComponentBase*)NULL;
                ComponentBase *SC = (ComponentBase*)aTypes->Item(cindex)->Data();
                wxCHECK2_MSG( SC, return wxID_CANCEL, _("Component item not found in the list"));
                SpectrumBase *SP = (SpectrumBase*) TheSpectrum;
                wxBusyCursor wait;
                SC->PreCreate(SP);
                SC->CD = SC->CreateComponentDialog(SP->GetView());
                if(SC->CreatePrepared())
                {
                    OP = (ComponentBase*)SC->Clone();   // Create the new component
                    SC = SP->GetActiveComponent(); // Get the old (empty) component
                    OP->UpdateClonedCopy(*SC);       // Copy some settings from the old component
                    if(!OP->Setup())
                    {
                        delete OP;  // Setup not succeeded, delete erroneous copy
                        return wxID_CANCEL;
                    }
                    (SP->Components->Item(SP->GetActiveComponentNo()))->SetData(OP);// Replace with new
    //                delete SC; // Delete obsolete, empty component // Done now in Undo/Redo
                    OP->SaveToBatchFile(CommandCode , aTypes, cindex);
                    OP->PostCreate();
                    return wxID_OK;
                }
                else
                {
                    return wxID_CANCEL;
                }
            }
            else
             wxMessageBox(_("Press OK to continue"), _("Cannot create component in the root window"),
                wxOK | wxCANCEL | wxICON_HAND, NULL) ;
        }
        else
            wxMessageBox(_("Press OK to continue"), _("Must be empty or the same type"),
                wxOK | wxCANCEL | wxICON_HAND, NULL) ;
        return wxID_CANCEL;
    }//ComponentBase::Create


    // Virtual function, to create comp. parameters from batch file
    // Called from CreatePrepared
    void    ComponentBase::CreateFromBatch(void)
    {
        ComponentFlag = BatchProcessor.GetLong();
        FeatureFlag = BatchProcessor.GetLong();
    }//Component1Dim::CreateFromBatch


    // Virtual function, to set up component parameters
    bool    ComponentBase::CreatePrepared(void)
    {
        bool Pr = true;
        if(BatchProcessor.GetCurrentLevel())
            CreateFromBatch();
        else
            {
                wxASSERT_MSG(CD->ResizeDialog(),"ComponentDialog creation failed");
                CreateSetParameters();
                if(wxID_OK ==CD->ShowModal())
                    CreateGetParameters();
                else
                    Pr = false;
            }
        return Pr;
    }

    // virtual function, to set up parameters shown in component dialog
    // Called from CreatePrepared
    void    ComponentBase::CreateSetParameters(void)
    {
    }//ComponentBase::CreateSetParameters


    // virtual function, to read out parameters shown in component dialog
    // Called from CreatePrepared
    void    ComponentBase::CreateGetParameters(void)
    {
    }//ComponentBase::CreateGetParameters

    // Write out component to text stream
    void ComponentBase::SaveToBatchFile(int CommandCode, wxList *aTypes, size_t &cindex)
    {
        BatchProcessor.AddLine(CommandToString(CommandCode, aTypes, cindex));
    }

    /** Write out component creation commands in a string
        \param  CommandCode code for the current command
        \param  aTypes      list of valid subcomponents
        \param  cindex      Index of current selection in the list
    */
    wxString ComponentBase::CommandToString(int CommandCode, wxList *aTypes, size_t &cindex)
    {
        wxString s;
        SpectrumBase* SP = (SpectrumBase*)GetSpectrum();
        wxFrame *F = (wxFrame*)SP->GetView()->GetFrame();
        wxString SubType;
        wxCHECK2_MSG(cindex<aTypes->GetCount(), return s, "Illegal subtype index");
        SubType = ((ComponentBase*)aTypes->Item(cindex)->Data())->GetClassName().AfterFirst('|');
        if((NMin!=0) || (NMax!=SP->GetNoOfEPoints()-1))
        {   // Window is zoomed, write it out
    //      SetDisplayZoomSub(SP->GetView(),VEMin,VEMax,VCMin,VCMax);
        }
    // Type out marker position, if necessary
        if((GetMrkL()!=NMin) || (GetMrkR()!=NMax))
        {
            AxisRecord *EAxis = GetAxisE();
            wxString ua = wxString::Format("|%s|%s\n",EAxis->GetFormatted(GetE(GetMrkL())),EAxis->GetFormatted(GetE(GetMrkR())));
            s << BatchProcessor.GetMenuPath(F, SET_DISPLAY_MARKER) + ua;
        }
    //    if(-1 == CommandCode)
    //        CommandCode = IsContributing() ? COMP_CHANGE : COMP_CHANGE_PSEUDO;
        s << BatchProcessor.GetMenuPath(F, CommandCode) + wxString::Format("|%s|0x%x|0x%x\n",SubType,ComponentFlag,FeatureFlag);
        s << BatchProcessor.GetMenuPath(F, COMP_CHANGE_LABEL) + "|" + GetLabel()+'\n';  // Type label
        s << ToString(BatchProcessor.GetMenuPath(F,COMP_CHANGE_ADJUST));    // Type adjusted parameters
        return s;
    }

    extern wxList	EWABackgroundTypes;
    extern wxList	EWAPeakTypes;
    extern wxList	EWAMultiLineTypes;
    extern wxList	EWATailTypes;

    // Write out component to text stream
    void ComponentBase::SaveToTextStream(wxTextOutputStream *TS)
    {
        if(!TS)
            return; // We do not have an open stream
        SpectrumBase* SP = (SpectrumBase*)GetSpectrum();
        wxFrame *F = (wxFrame*)SP->GetView()->GetFrame();
        if((NMin!=0) || (NMax!=SP->GetNoOfEPoints()-1))
        {   // Window is zoomed, write it out
    //      SetDisplayZoomSub(SP->GetView(),VEMin,VEMax,VCMin,VCMax);
        }
    // Type out marker position, if necessary
        if((GetMrkL()!=NMin) || (GetMrkR()!=NMax))
            SetDisplayMarkerSub(SP->GetView(), GetMrkL(), GetMrkR());
        int CommandCode = -1;   // The command code
        switch(GetType())
        {
        case ot_Bckgnd:
            CommandCode = COMP_CHANGE_BACKGROUND;
            break;
        case ot_Peak:
            CommandCode = COMP_CHANGE_PEAK;
            break;
        case ot_Multiline:
            CommandCode = COMP_CHANGE_PSEUDO_MULTILINE;
            break;
        case ot_Tail:
            CommandCode = COMP_CHANGE_PSEUDO_TAIL;
            break;
        default:
            wxASSERT_MSG(false, "Illegal component type");
        }
        wxString SubType = GetClassName().AfterFirst('|');

        TS->WriteString(BatchProcessor.GetMenuPath(F, CommandCode) + "|" + wxString::Format("%s|0x%x|0x%x\n",SubType,ComponentFlag,FeatureFlag));
        TS->WriteString(BatchProcessor.GetMenuPath(F, COMP_CHANGE_LABEL) + "|" + GetLabel()+'\n');
        WriteToFile(TS,BatchProcessor.GetMenuPath(F,COMP_CHANGE_ADJUST));
    }

    /** The member calculates the attached components from this one
        and stores the corresponding contribution in the 'ObjectStore'
        of the master component
    */
    void    ComponentBase::PostCalculate(void)
    {
        for (size_t i = 0; i<GetAttachmentListCount(); i++)
        {
            long ID = GetAttachmentListItem(i);
    //        ComponentBase *CS = (ComponentBase*)GetAttachmentListItem(i);
            ComponentBase *CS = SpectrumProcessor.FindComponentByUniqueID(ID);
            wxCHECK_RET(CS,"Attached comp not found");
            CS->Calculate(this);
        }
    }//PostCalculate

    void ComponentBase::PreCreate(SpectrumBase *aSpectrum)
    {   ComponentBase*  SC = (ComponentBase*)(aSpectrum->GetActiveComponent());
        TheSpectrum = aSpectrum; UniqueID = SC->UniqueID;
        NMin = SC->NMin;    NMax = SC->NMax;
        AlphaMode = SC->AlphaMode;
        //ROIList : PCollection;    { of TROIs }
        //ActiveROI : word;         // The currently active ROI
        ILow = SC->ILow; IHigh = SC->IHigh;
        EZoom = SC->EZoom; EZoomBegin = SC->EZoomBegin;
        VEMin = SC->VEMin; VEStep = SC->VEStep; VEMax = SC->VEMax;
        ObjectSize = ObjectBegin = 0;
        XEnergy = -1.F;
    //  for (int i=0; i<gsMarker+1; i++)
    //        GraphStyle[i] = SC->GraphStyle[i];
    }// ComponentBase::PreCreate

    void    ComponentBase::PostCreate(void)
    {   if(CD)
            delete CD; //Destroy ComponentDialog
        CD = (ComponentDialog*)NULL;
    }

    /// Finalize the effect of the new parameters
    void ComponentBase::FinalizeData(wxView *MyView)     ///< Finalize the effect of the new parameters
    {
        CalculateMerit();                       // Calculate the new fit quality
        if(MyView)
        {
            SpectrumView *SW = (SpectrumView *)MyView;
            SW->DisplayMerit();
            SW->GetCanvas()->Refresh();                 // Refresh the screen with the new data
            SW->GetCanvas()->Update();
        }
    }

    /** Return number of points actually used in the fit
        i.e. accounts for excluded regions
    */
    size_t ComponentBase::GetNoOfFittedPoints(void)
    {   size_t N = 0;
        size_t i = NMin; size_t j; if(-1==(int)i) i = 0;   // Correct for measurement
        wxNode *node = TheSpectrum->Excluded->First();
        while(node)
        {   wxPoint *P = (wxPoint*) node->Data();
            N += CalculateFittedPointsPart(i, P->x-1);
            i = P->y +1; node = node->Next();
        }// Now all exluded regions processed, finish it
        j = NMax;
        N +=CalculateFittedPointsPart(i,j);
        return N;
    }//ComponentBase::GetNoOfFittedPoints



    double ComponentBase::GetSumOfSquares(void)
    {
        return 0.0;
    }//ComponentBase::GetSumOfSquares

    /// Add parameter mode information
    wxString ComponentBase::AddParameterModeInformation(unsigned int index)
    {
        wxASSERT_MSG(index < GetCount(),"Illegal parameter index");
        wxString S;
        if(pmNoLink != GetLinkMode(index))
        {
            S += " Slave";  // It is linked
            S += wxString::Format("(%c",Parameters[index].GetLinkModeChar() );
            if(pmReplaceLink != Parameters[index].GetLinkMode())
                S += wxString::Format("%9s",Parameters[index].GetFormattedItem(pmValue,true) );
            S += ")";
        }
        if(IsMaster(index))
            S += " Master";  // It is master
    //    if(IsFlagSet(index, pmFixedValue))
    //        S += " Fixed";  // It is fixed
        if(S.Length()>0)
            S = '[' + S + ']';
        // Now add parameter bound position information
        float low = Parameters[index].GetLimL(); float high = Parameters[index].GetLimH();
        float step = Parameters[index].GetdBase();
        int ccount = 20;    // Length of character bar
        S += '<';
        bool IsLog = IsFlagSet(index, pmLogDisplay);
        if((fabs(high-low)<step) || // The range is unreasonably small
            ( IsLog && ((high <= 0) || (low <= 0)))) // or the log NAN
        {
            S += "????????????????????";
        }
        else
        {
            double value = Parameters[index].GetParameterValue();
            char posc, fillc;
            if( (fabs(value-low)<step) || (fabs(value-high)<step) )
            {   // Parameter is 'on bound'
                fillc = '*'; posc = '!';
            }
            else
            {
                fillc = '-'; posc = '*';
            }
            double norm = IsLog ? (ccount-1) / (log(high)-log(low)) : (ccount-1) / fabs(high-low);
            int pos = IsLog ? (log(value)-log(low))*norm+0.51 :fabs(value-low)*norm+0.51;
            for(int i = 0; i<ccount; i++)
                S += pos == i ? posc : fillc;
        }
        S += '>';
        if(IsFlagSet(index, pmLogDisplay))
            S += " Log";
        return S;
    }

    //Return basic information of ComponentBase
    void ComponentBase::GetText(wxStringList **T)
    {
        if(IsRoot())// Return text belonging to this component
            // it is the root window, get spectrum text
            TheSpectrum->GetText(T);
        else
        {
            (*T) = new wxStringList;
            size_t Index;
            (*T)->Add(wxString::Format(_("Window#%d:%s"), GetUniqueID(), GetLabel()));
            if(Parameters)
            {// write out component parameters
                (*T)->Add(wxString::Format(_("Contains component %s with %.4i parameters"),GetLabel(), NoOfItems));
                for( Index = 0; Index < NoOfItems; Index++)
                {
                   wxParameterRecord *MyPar= &(Parameters[Index]);
                   wxString A;
                   if(Parameters[Index].GetFlags() & pmFixedValue)
                       A = _(",fixed ");
                   else
                       A = (_("+/-") + Parameters[Index].GetFormattedItem(pmStep));
                   / *",fixed " : _("+/-") + Parameters[Index].GetFormattedItem(pmStep);
                   wxString A = (Parameters[Index].GetFlags() & pmFixedValue)
                   ? _(",fixed ")  : (_("+/-") + Parameters[Index].GetFormattedItem(pmStep));/ *",fixed " : _("+/-") + Parameters[Index].GetFormattedItem(pmStep);
        * /            wxString B = "[" + Parameters[Index].GetFormattedItem(pmLimL) + "," +
                                Parameters[Index].GetFormattedItem(pmLimH)+ "]";
                    if(Parameters[Index].GetFlags() & pmIntegerOnly) B += _(" <Integer>");
                    (*T)->Add(wxString::Format(_("%.3i:%s%s%s%s"), Index, MyPar->GetLabel(), Parameters[Index].GetFormattedItem(pmValue), A, B ));
                    if(Parameters[Index].GetMasterPointer())
                        (*T)->Add(wxString::Format(_("     (%s:%s)"),Parameters[Index].GetLinkedToString(), Parameters[Index].GetMasterPointer()->Parameters[1].GetFormattedItem(pmValue)));
                }
            }
        }
    }//ComponentBase::GetText

    / ** Calculate active and total number of parameters for the component
        \param  Total   total number of parameters in the component
        \param  Active  total number of free parameters in the component
    * /
    void    ComponentBase::GetTotalNoOfParameters(size_t &Total, size_t &Active)
    {
        if(Parameters)
            {   Active = 0;
                for(size_t i = 0; i<NoOfItems; i++)
                    if(!(Parameters[i].IsFixed())) Active ++;
                Total = NoOfItems;
            }
            else // this is a no-parameter object
            { Total = 0; Active = 0;}
    }//of ComponentBase::GetTotalNoOfParameters

    //    \return the root component if any or NULL
    ComponentBase*  ComponentBase::GetRoot(void)// return the root component
    {   return TheSpectrum ? TheSpectrum->GetRoot() : (ComponentBase*)NULL;}

    / ** The procedure calculates the first "rounded" value and the step
        between the values for the interval given by Lowlimit and Highlimit
        according to the Y scaling given by Mode
        \param	LowLimit	the lower value to scale
        \param	HighLimit	The higher value to scale
        \param	First		Will return the calculated new lower value
        \param	Step		Will return the calculated step size
        \param	Last		Will return the calculated new high value
        \param	Mode		Flag word to control scaling
    * /
    void ComponentBase::Scale(float LowLimit, float HighLimit,
                              float &First, float &Step, float &Last,
                              amAxisMode Mode)
    {   float mt,ex,range,s;
        int ls,ls1;
        if(HighLimit==LowLimit) HighLimit += LowLimit; //prevent normalization problem
        range = HighLimit-LowLimit;
        if (Mode & am_Trnsfrm)
        { // we make either root or logarithmic scaling }
            ex = LowLimit>0 ? (float)log10(LowLimit)-.49 : 0.0F;
            ls = (int)(ex+100); ls -= 100;
            while(exp10((double)ls)>(HighLimit-LowLimit)/3) ls--;
            Step = (float)exp10((double)ls); //First = Step;
            ls1 = (int)((LowLimit-Step)/Step -1) ; First = ls1*Step;
            while((First+Step <= LowLimit) || ((am_Log == (Mode & am_Log)) && (0==First)))
                First += Step;
            ex = (float)log10((double)HighLimit); ls1 = (int)(ex+101); ls1 -= 100;
            Last = (float)exp10((double)ls1); ex = (float)exp10((double)ex-ls1);
            ex = (ex<=0.3) ? Last/10 : Last;
            if(ex>range) ex = Step;
            ls1 = (int)((HighLimit+Step)/Step +1); Last = ls1*Step;
            while(Last-ex>HighLimit) Last -= ex;
            if((Mode & am_Sqrt) && (LowLimit<0))
                { First = 0;}
            if((Mode & am_Log) && (LowLimit<=0))
                {First = Step;}
        }
        else
        {// we make a linear scaling
            ls = (int)(log10(fabs(range))+100); ex = (float)(sign(range)*exp10((double)ls-100));
            mt = range/ex;
            if(mt<1.5F) s = 0.1F;
            else    if(mt<=3.0F) s = 0.2F;
                    else    if(mt<=7.0F) s = 0.5F;
                            else s = 1.0F;
            Step = s*ex;
            ls = (int)(LowLimit/Step-0.99999F);
            if(!(sign(LowLimit)==sign(Step)))
                First = ls*Step;
            else First=int(LowLimit/Step)*Step;
            ls = (int)(HighLimit/Step);
            if(!(sign(HighLimit)==sign(Step)))
                Last=ls * Step;
            else Last=int(HighLimit/Step+0.99999F)*Step;
        };
        if(!(Mode & am_Rounded))
            {   First = LowLimit; Last = HighLimit;}
    }//ComponentBase::Scale



    / **
         Creates a dialog box, with controls field and buttons field, complete with sizers
    * /

    ComponentDialog::ComponentDialog(wxWindow *parent, wxWindowID id, const wxString& title,
                    const wxPoint& pos , const wxSize& size ,
                    long style , const wxString& name ):
    UserDialog( parent, id, title, pos, size, style, name)
    {
        CheckBoxes = new wxCheckBox*[32];
        wxStaticBox *pmbox = new wxStaticBox(this, -1, _("Parameters"));
        chboxsizer = new wxStaticBoxSizer( pmbox, wxVERTICAL );
        controlinternal->Add(chboxsizer, 1, wxEXPAND | wxALL , 1);
    }//ComponentDialog


    / ** Adjust a component (parameter set)
        \param  par the component to adjust
        \param  view  the view to display component in
        \param  caption caption for the adjust dialog
        \param  parent  parent window of the dialog * /
    int wxAdjustComponent( ComponentBase *par, SpectrumView *view, const wxString caption, wxWindow *parent)
    {
        wxAdjustComponentBox *dialog = new wxAdjustComponentBox(view->GetFrame(),-1, caption + " " << par->GetLabel());
        dialog->Create(par, view);
        int rc = dialog->ShowModal();               // Show the dialog to play with
        delete dialog;
        return rc;
    }
#endif
