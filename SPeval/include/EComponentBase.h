/*! @brief ComponentBase for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef ESsistMe_EComponentBase_H
#define ESsistMe_EComponentBase_H
#include <string>
#include <iostream>
#include <vector>
#include "MinuitParameter.h"
#include "ESpectrumBase.h"
using namespace std;


/*! @brief Base class for storing spectral data */
class EComponentBase : public MinuitParameterSet
{
public:
  EComponentBase(ESpectrumBase *aSpectrum, int aWIdent, bool Root, const string& aLabel);
  /// Return temporary store
  inline const double*   GetObjectStore(void) const
      {   return ObjectStore.data();}
protected:
  bool Setup(void);
  /// Reserve storage for some data
  void    GetStorage(int iFrom, int iTo);
  ESpectrumBase    *m_Spectrum;   ///< Back pointer to the owner spectrum, a copy
  int ObjectBegin;  // The offset value of the data storage vector
  vector<double> ObjectStore;   // Vector for storing component data
};


#endif //  ESsistMe_EComponentBase_H
#if defined(OldWX)
    //: compbase.h
    /*  Component base types
        \author   (c) JÃ¡nos VÃ©gh, MTA ATOMKI, Debrecen, Hungary (veghj@users.sf.net)
        \since    Oct 17, 2004
        $Id: compbase.h,v 1.1 2004-08-23 14:35:15+02 veghj Exp veghj $
     */

    #if defined(__GNUG__) && !defined(__APPLE__)
        #pragma interface "compbase.h"
    #endif

    #ifndef ewa_compbaseh
    #define ewa_compbaseh

    #include <wx/wxprec.h>

    #ifdef __BORLANDC__
        #pragma hdrstop
    #endif

    #ifndef WX_PRECOMP
        #include <wx/wx.h>
    #endif

    #include    <iostream.h>
    #include    <wx/tokenzr.h>
    #include    <wx/datetime.h>
    #include    <wx/cmdproc.h>
    #include    <wx/txtstrm.h>
    #include    "xbool.h"
    #include	"utils.h"
    #include    "paradjst.h"

    #include    "spwindow.h"
    #include    "specbase.h"


    #define   gsMeasured			0x0
    #define   gsFitted				0x1
    #define   gsComponent			0x2
    #define   gsMarker				0x3

    class ComponentDialog;  // forward decl.

    ///    The anchestor of all kind of spectrum components.
    class ComponentBase : public wxParameterSet
    {
    DECLARE_DYNAMIC_CLASS(ComponentBase)
    public:
        float   EZoom,              ///< Current zoom factor
                EZoomBegin;         ///< Initial zoom factor
        float   CZoom,              ///< The current C-zoom
                CZoomBegin;         ///< The initial C-zoom
        float   VEMin,              ///< The minimum E value
                VEStep,             ///< E value step
                VEMax;              ///< The maximum E value
        float   VCMin,              ///< The minimum C value to display
                VCStep,             ///< The C step size to display
                VCMax;              ///< The maximum C value to display
        /// Create a component for aSpectrum, with ID aWIdent and label aLabel
        ComponentBase(SpectrumBase *aSpectrum, const int aWIdent, const bool Root, const wxString aLabel="");
        /// Make a full copy of the component base
        ComponentBase(const ComponentBase &C);
        /// Default ctor for ComponentBase.
        ComponentBase(void);
        /// Dtor for ComponentBase
        virtual ~ComponentBase(void);
        /// Creates and returns a full copy of the base object
        /**	\return	the created new base component */
        virtual wxParameterSet *Clone(void) const
              { return new ComponentBase(*this);}
        /// Update some settings of a cloned component from the replaced component C
        virtual void UpdateClonedCopy(const ComponentBase &C);
        /// Initialize the component to default state
        void    Init(const bool Root);
        /// Adjust the parameters of the component
        virtual int Adjust(SpectrumView *view = NULL);
        /// Return true if this set is linked already to P
        bool IsLinkedTo(long ID);
        /// Link this parameter set to another parameter set
        void LinkTo(long ID);
        /// UnLink this parameter set from the another one
        void UnLinkFrom(long ID);
        /// Calculates the merit number for the component
        /*	\return the calculated merit number */
        virtual double CalculateMerit(void)
            {   wxASSERT(false); return 0.0;}
        /// Calculate fitted points between ith and jth points. inclusive
        size_t CalculateFittedPointsPart(const size_t i, const size_t j);
        /// Select a component from the subtype list and create it
        virtual int     Create(int CommandCode, wxList *aTypes, size_t &cindex);
        /// Get parameters from the parameter select dialog
        virtual void    CreateGetParameters(void);
        /// Set parameters for the  parameter select dialog
        virtual void    CreateSetParameters(void);
        /// Call after creating a component
        virtual void    PostCreate(void);
        /// Call before creating a component
        virtual void    PreCreate(SpectrumBase *aSpectrum);
        /// Save component to text file
        virtual void    SaveToTextStream(wxTextOutputStream *TS);//,
    //                                     int CommandCode, wxList *aTypes, size_t &cindex);
        /// Save component to batch file
        virtual void    SaveToBatchFile(int CommandCode, wxList *aTypes, size_t &cindex);

        /// Return the C axis of the spectrum
        AxisRecord *GetAxisC(void);
        /// Return the E axis of the spectrum
        AxisRecord *GetAxisE(void);
        /// Return current E-axis mode
        inline amAxisMode GetAxisModeE(void) const
            {   return AxisModeE;}
        /// Return class name
        inline wxString GetClassName(void) const
            {   return ClassName;}
        /// Return view attached to the spectrum of the component
        inline SpectrumView*    GetView(void) const
            {   return TheSpectrum->GetView();}
        /// Return ID of the component
    //    inline int      GetComponentID(void)
    //		{   return  ComponentID;}
        /// Return component type
        inline ot_ComponentTypes GetType(void) const
            {	return  ComponentType;}
        /// Return component type
        inline ot_ComponentSubTypes GetSubType(void) const
            {	return  ComponentSubType;}
        /// Return correlation factor
        inline virtual  double  GetCorrelation(void) const
            {	return AC1;}

        /// Remove affected data, with old parameter
        virtual void SubtractOldData(void)
            {   SubtractFromFit(); }
        /// Add the new affected data, with new parameterset
        virtual void AddNewData(void)
            {   AddToFit();  }
        virtual void FinalizeData(wxView *MyView = NULL);     ///< Finalize the effect of the new parameters
        /// Return degrees of freedom
    //    virtual size_t GetDegreesOfFreedom(void) const
      //      { return GetNoOfFittedPoints() - GetNoOfFreeParameters();}
        /** Add the component to the fitted envelope
            Must be overwritten in derived classes.
            \param  S   Pointer to the spectrum */
        virtual void AddToFit(SpectrumBase *S = NULL)
            {   wxASSERT_MSG(false,"Adding ComponentBase to Fit"); }
        /** Subtract this component from the fit
            Must be overwritten in derived classes.
            \param  S   Pointer to the spectrum */
        virtual void SubtractFromFit(SpectrumBase *S = NULL)
            {   wxASSERT_MSG(false,"Subtracting from SpectrumBase"); }
        /// Return the energy at the ith data point
        virtual float GetERaw(const int i) const
            {   //(*GetECurrent)(this,i);
                return TheSpectrum->GetE(i);
            }
        /// Set the energy at the ith data point
        virtual void SetE(const int i, const float E)
            {   //(*GetECurrent)(this,i);
                TheSpectrum->SetE(i,E);
            }
        /// Return current energy; depending on scale type
        virtual float GetE(const int i)
            {   return  (IsOnBindingScale() == GetRoot()->IsOnBindingScale())
                        ? TheSpectrum->GetE(i) : XEnergy - TheSpectrum->GetE(i);
            }
        /// Get the binding energy at point I
    /*    float   GetBE(const int i)
            { return GetKE(TheSpectrum->GetE(i));   }
      */  /// Get the binding energy corresponding to E
    /*    float   GetBE(const float E)
            { return (IsOnBindingScale() == xboolean::False)
                ? XEnergy - E : E;
            }
    */
        /// Get ILow
        inline size_t GetILow(void) const
            {	return ILow;}
        /// Get IHigh
        inline size_t GetIHigh(void) const
        {	return IHigh;}
        /// Get NMin
        inline size_t GetNMin(void) const
        {	return NMin;}
        /// Get NMax
        inline size_t GetNMax(void) const
        {	return NMax;}
        /// Set ILow
        inline void SetILow(const size_t N)
        {	ILow = N;}
        /// Set NMin
        inline void SetIHigh(const size_t N)
        {	IHigh = N;}
        /// Set NMin
        inline void SetNMin(const size_t N)
        {	NMin = N;}
        /// Set NMax
        inline void SetNMax(const size_t N)
        {	NMax = N;}
        /// Delete the storage reserved for the component
        /// Reserve storage for the component
        void    FreeStore(void)
        {
            if(ObjectStore)
                delete [] ObjectStore;
            ObjectStore = NULL;
        }
      // Calculation-related members
        /// Calculate the given component, alone
        virtual void Calculate(void)
        {
            HasChanged = false;
            if(ObjectStore)
                memset(ObjectStore,0, TheSpectrum->GetNoOfEPoints()*sizeof(ObjectStore[0]));
        }
        /// Calculate the given component as attachment
        virtual void Calculate(ComponentBase* C)
            {	wxASSERT(false);};  // For attachments
        /// Call after calculating a component, say to calculate the attachments
        virtual void    PostCalculate(void);
        /// Call before calculating a component, say to prepare some operations
        virtual void    PreCalculate(void)
        {}
        /// Create the component from batch file
        virtual void    CreateFromBatch(void);
        /// Prepare creating component
        virtual bool    CreatePrepared(void);
        /// Get the kinetic energy at point I
        /// Create a component dialog for parametrization
        virtual ComponentDialog* CreateComponentDialog(SpectrumView *view)
            {	return NULL;};
        /// Write out component creation commands in a string
        wxString CommandToString(int CommandCode, wxList *aTypes, size_t &cindex);
        /// Add parameter mode information for the index-th parameter
        wxString AddParameterModeInformation(unsigned int index);
    /*    float   GetKE(int i)
            { return GetKE(TheSpectrum->GetE(i));   }
    */    /// Get the kinetic energy corresponding to E
    /*    float   GetKE(float E)
            { return (IsOnBindingScale() == xboolean::True)
                ? XEnergy - E : E;
            }
    */    /// Set the binding energy at point I
    /*    void    SetBE(int i, float E)
            { TheSpectrum->SetE(i, (IsOnBindingScale() == xboolean::False) ? XEnergy - E : E); }
    */    /// Set the kinetic energy at point I
    /*    void    SetKE(int i, float E)
            { TheSpectrum->SetE(i, (IsOnBindingScale() == xboolean::True) ? XEnergy - E : E); }
    */
        /// Return the "Component" flag
        inline int GetComponentFlag(void) const
            {  return ComponentFlag;}

        /// Return number of points available
        virtual size_t GetNoOfPoints(void) const
            {	return NMax -NMin +1;}
        /// Return number of points used in the fit
        virtual size_t GetNoOfFittedPoints(void);
        /// Return the calculated merit number
        virtual double  GetMerit(void) const
            {	return Merit;}
        /// Return the root component
        ComponentBase*  GetRoot(void);
        /// Set if the components is the root of a spectrum
        inline void SetRoot(const bool b = false)
            {   m_Root = b;}
        /// Return pointer to the spectrum
        inline SpectrumBase *GetSpectrum() const
            { return TheSpectrum;}
        /** Return the sequence number corresponding to value Z
            \param  E   the value of the energy to search for
            \return the index of the point with energy nearest to E */
        int GetIndexOfE(float E)
        {   return (IsOnBindingScale() == GetRoot()->IsOnBindingScale())
                ? TheSpectrum->GetIndexOfE(E) : TheSpectrum->GetIndexOfE(XEnergy-E);
        }
        /// Return sum of squares
        virtual double GetSumOfSquares(void);
        /// Add some new string to the already collected string list
        virtual void GetText(wxStringList **T);
        /// Return if component displayed in alpha mode
        inline bool GetAlphaMode(void)
            {	return AlphaMode;}
        /// Return the excitation energy
        inline float GetXEnergy(void) const
            {   return XEnergy; }
        /// Return true if the excitation energy is known
        inline bool IsXEnergyKnown(void) const
            {   return XEnergy>0; }
        /// Return true if the component is root
        inline bool IsRoot(void) const
            {   return m_Root; }
        /// Return of the component is contributing
        inline bool IsContributing(void)
            {	return Contributing;}
        /// Return if the _component_ is on binding scale
        inline  xboolean::xbool &GetOnBindingScale()
            {   return  OnBindingScale;}
        /// Return if the _component_ is on binding scale
        inline  bool IsOnBindingScale()
            {   return  xboolean::True == OnBindingScale;}
        /// Return if the _component_ is on binding scale
        inline  bool IsOnKineticScale()
            {   return  xboolean::False == OnBindingScale;}
        /// Return number of active/total parameters
        void    GetTotalNoOfParameters(size_t &Total, size_t &Active);
        /// Will be deprecated
        /// Scale the region to some "nice, round" values
        void    Scale(float LowLimit, float HighLimit,
                    float &First, float &Step, float &Last, amAxisMode Mode);
        ///	Sets the alphanumeric display mode for the component
        inline void SetAlphaMode(const bool M)
            {   AlphaMode = M;}
        /// Return C axis mode
        inline amAxisMode GetAxisModeC(void)
            {    return  AxisModeC;}
        /// Set C-axis mode
        inline void SetAxisModeC(const amAxisMode M)
            {    AxisModeC = M;}
        /// Sets the axis mode for energy axis
        inline void SetAxisModeE(const amAxisMode M)
            { AxisModeE = M;}
        /// Sets binding energy scale according to parameter B
        /*	\param	B	the new state of the BE scale info.
            \see xbool
            \see OnBindingScale
        */
        inline void SetOnBindingScale(xboolean::xbool B)
            {   OnBindingScale = B;}
        /// Set spectrum for the component
        /** \param  s   the parent spectrum of the component */
        void    SetSpectrum(SpectrumBase *s)
            { TheSpectrum = s;}
        /// Setup the component base object
        /** \ return    true if the spectrum set up successfully */
        virtual bool    Setup();
        /// Set the excitation energy
        /** \param  X   Then new value of the excitation energy */
        inline void SetXEnergy(float X)
            {   XEnergy = X;}
        /// Return the value of the legt marker
        inline size_t  GetMrkL(void) const
            {  return MrkL;}
        /// Return value of the right marker
        inline size_t  GetMrkR(void) const
            {  return MrkR;}
        /// Return value of the object begin
        inline size_t  GetObjectBegin(void) const
            {  return ObjectBegin;}
        /// Return value of the object size
        inline size_t  GetObjectSize(void) const
            {  return ObjectSize;}
        /// Set left marker
        inline void SetMrkL(const size_t L)
            {   MrkL = L;}
        /// Set right marker
        inline void SetMrkR(const size_t R)
            {   MrkR = R;}
    protected:
        bool    m_Root,                 ///< TRUE if the component is the root of the spectrum components
                Contributing,           ///< TRUE for contributing components like peak and background
                AlphaMode;              ///< TRUE if to be displayed in alphanumeric mode

        wxString    ClassName;          ///< Store class name here
        ot_ComponentTypes ComponentType;      ///< Number identifying component type
        ot_ComponentSubTypes ComponentSubType;///< and subtype
        //ROIList : PCollection;    { of TROIs }
        //ActiveROI : word;             // The currently active ROI
        xboolean::xbool OnBindingScale; ///< If energy data are stored on BE
    //    int     ComponentMode;          ///< The component mode flag word
        amAxisMode   AxisModeC;         ///<  Stores component display lookout
        amAxisMode  AxisModeE;          ///<  Stores component display lookout
        int     Index0 ;                ///<  Just a work variable
        double  Merit,                  ///< The calculated merit value, usually chisq
                AC1;                    ///< The calculated autocorrelation value value
        float   XEnergy;                ///< Energy of the exciting source : -1 = absent
        SpectrumBase    *TheSpectrum;   ///< Back pointer to the owner spectrum, a copy

        size_t  NMin,                   ///< Lowest sequence# of points
                NMax;                   ///< Highest sequence# of points
        size_t  ILow,                   ///< to calculate the component from this
                IHigh;                  ///< to calculate the component up to this
        int     ComponentFlag,      ///< Contains 'or'ed fbgx_xx values
                FeatureFlag;        ///< Contains features like etc
        size_t  MrkL,               ///< Left marker
                MrkR;               ///< Right marker
        float   *ObjectStore;       ///< object storage if necessary
        size_t ObjectSize,          ///< object size, in elements
            ObjectBegin;            ///< 1st channel of the stored object
        ComponentDialog *CD;        ///< The component parameter select dialog
    };// of class ComponentBase


    ///  component class parameter select dialog
    class ComponentDialog : public UserDialog
    { public:
        /// Ctor for the component dialog
        ComponentDialog(wxWindow *parent, wxWindowID id, const wxString& title,
                    const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                    long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = "dialogBox");
        wxCheckBox  **CheckBoxes;   ///< checkbox array for the parameter select boxes
        wxStaticBoxSizer *chboxsizer;   ///< Space for the checkboxes
        wxStaticBoxSizer *optionssizer;   ///< Space for the component options
    };

    /// Adjust a component (parameter set)
    int wxAdjustComponent( ComponentBase *par, SpectrumView *view, const wxString caption, wxWindow *parent);

    #endif // ewa_compbaseh
#endif // oldWX
