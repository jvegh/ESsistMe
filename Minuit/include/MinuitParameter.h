#ifndef MN_MinuitParameter_H_
#define MN_MinuitParameter_H_

#include <cstring>
#include <algorithm>
#include <memory>
// Define this to have extensions like linking and parameter sets
#define MINUIT_EXTENDED 1
#include    <cassert>

#if MINUIT_EXTENDED

class MinuitParameterSet;   // forward declaration
#include <vector>
#include    <cmath>

#define ASSERT_MSG(what,msg) if(!(what)) {printf("debug : Parameter : %s\n",msg);::exit(0);} 
/*  as wxCHECK2 but with a message explaining why we fail */
#define CHECK2_MSG(what, op, msg)  if (!(what)) {printf("debug : Parameter : %s\n",msg); op; }
//#define MnPmNameLength 15
#else
#endif  // MINUIT_EXTENDED
#define MnPmNameLength 11

/*! @class	MinuitParameter
    
    Class for the individual Minuit parameter with name and number; 
    contains the input numbers for the minimization or the output result
    from minimization;
    possible interactions: fix/release, set/remove limits, set value/error; 
 */
#if MINUIT_EXTENDED
/*!
  The parameters can be linked in two different ways.
  -# In the first way the individual parameter records are linked together
        This mode links this _slave_ MinuitParameter of some MinuitParameterSet
        to some individual _master_ MinuitParameter of some MinuitParameterSet
        Each record contains a MasterPointer, pointing to the master MinuitParameterSet,
        MasterParameterNo gives the index of MinuitParameter in the _master_ MinuitParameterSet
        When linking, the _slave_ MinuitParameter will be stored in the
        _master_ 'MinuitParameterSet' AffectedList list
  -# In the second way the parameter sets are linked together:
        This mode links this _slave_ MinuitParameterSet to some other _master_ MinuitParameterSet.
        MasterPointer points to the attached MinuitParameterSet,
        MinuitParameterSet maintains two lists: 
            AttachmentList : holds the list of IDs of "MinuitParameterSet"s,
                which affect this one.
            AffectedList: holds the list of the MinuitParameterSet which are
                affected by this one.
*/
#endif // MINUIT_EXTENDED

class MinuitParameter {

public:
#if MINUIT_EXTENDED
  ///  pm attribute indices
    enum pmAttributeIndex{ pmLimLow = 0, pmValue, pmLimHigh, pmError, pmWidth, pmPrecision, pmIntegerForm };
  /// Parameter record link modes
    enum pmLinkMode{
        pmNoLink    =0x00,          ///< The parameter is NOT linked
        pmReplaceLink,              ///< = Master(MasterParNo)
        pmAddLink,                  ///< = This[ParNo] + Master[MasterParNo]
        pmMultiplyLink,             ///< = This[ParNo] * Master[MasterParNo]
        pm1LinkHigh = pmMultiplyLink,///< The highest possible 1-pm linking mode
        pm2ParLinkMask = 0x10,      ///< The mode bit signaling 2-pm linking
        pm2LinearLink,              ///< = This[ParNo] * Master[MasterParNo+1] + Master[MasterParNo]
        pm2FWHMLinearLink,          ///< = This[0] * Master[MasterParNo+1] + Master[MasterParNo]
        pm2LinkHigh = pm2FWHMLinearLink ///< The highest possible 2-pm linking mode
    };
  /// Character representation of the link modes
#define    pmNoLinkChar             '@'
#define    pmReplaceLinkChar        '='
#define    pmAddLinkChar            '+'
#define    pmMultiplyLinkChar       '*'
#define    pm2LinearLinkChar        'L'
#define    pm2FWHMLinearLinkChar    'W'
#endif // MINUIT_EXTENDED
char pmLinkChars[4] = {pmNoLinkChar,pmReplaceLinkChar,pmAddLinkChar,pmMultiplyLinkChar}; 
std::string pmLinkStrings[4] = {("No link"), ("Replace"), ("Add"), ("Multiply") };

    /*!
   * @brief constructor for constant parameter
   * 
   * @param[in] num Number of the parameter
   * @param[in] name String ID of the parameter
   * @param[in] val Value of the parameter
   */
  MinuitParameter(unsigned int num, const char* name, double val) : 
    theNum(num), theValue(val), theError(0.), theConst(true), theFix(false), 
    theLoLimit(0.), theUpLimit(0.), theLoLimValid(false), theUpLimValid(false)
#if MINUIT_EXTENDED
    ,theWidth(8), theDecimals(2), LinkMode(pmNoLink), m_MySet(NULL), MasterPtr(NULL)
    ,MasterParameterNo(0), m_UserIndex(-1)
#endif // MINUIT_EXTENDED
    {
        setName(name);
    }
  
  /*!
   * @brief constructor for standard parameter
   * 
   * @param[in] num Sequence number of the parameter
   * @param[in] name String ID of the parameter
   * @param[in] val Value of the parameter
   * @param[in] err Error of the parameter
   */
  MinuitParameter(unsigned int num, const char* name, double val, double err) :
    theNum(num), theValue(val), theError(err), theConst(false), theFix(false), 
    theLoLimit(0.), theUpLimit(0.), theLoLimValid(false), theUpLimValid(false)
#if MINUIT_EXTENDED
    ,theWidth(8), theDecimals(2), LinkMode(pmNoLink), m_MySet(NULL), MasterPtr(NULL)
    ,MasterParameterNo(0),m_UserIndex(-1)
#endif // MINUIT_EXTENDED
    {
        setName(name);
    }
  
  /*!
   * @brief constructor for limited parameter
   * 
   * @param[in] num Number of the parameter
   * @param[in] name String ID of the parameter
   * @param[in] val Value of the parameter
   * @param[in] err Error of the parameter
   * @param[in] min Minimum value of the parameter
   * @param[in] max Maximum value of the parameter
   */
  MinuitParameter(unsigned int num, const char* name, double val, double err, 
          double min, double max) : 
    theNum(num),theValue(val), theError(err), theConst(false), theFix(false), 
    theLoLimit(min), theUpLimit(max), theLoLimValid(true), theUpLimValid(true)
#if MINUIT_EXTENDED
    ,theWidth(8), theDecimals(2), LinkMode(pmNoLink), m_MySet(NULL), MasterPtr(NULL)
    ,MasterParameterNo(0),m_UserIndex(-1)
#endif // MINUIT_EXTENDED
    {
        assert(min != max);
        if(min > max) {
        theLoLimit = max;
        theUpLimit = min;
        }
        setName(name);    
    }

  ~MinuitParameter() {}

  /*!
   * @brief Copy constructor
   * 
   * @param[in] par Parameter to be copied
   */
  MinuitParameter(const MinuitParameter& par) : 
    theNum(par.theNum), theValue(par.theValue), theError(par.theError),
    theConst(par.theConst), theFix(par.theFix), theLoLimit(par.theLoLimit), 
    theUpLimit(par.theUpLimit), theLoLimValid(par.theLoLimValid), 
    theUpLimValid(par.theUpLimValid) 
#if MINUIT_EXTENDED
    ,theWidth(par.theWidth), theDecimals(par.theDecimals), LinkMode(par.LinkMode)
    ,m_MySet(par.m_MySet), MasterPtr(par.MasterPtr),MasterParameterNo(par.MasterParameterNo),m_UserIndex(par.m_UserIndex)
#endif // MINUIT_EXTENDED
    {
        memcpy(theName, par.name(), MnPmNameLength*sizeof(char));
    }
  
#if MINUIT_EXTENDED
    /// This quasi-copy constructor makes a copy for the user-parameter vector
  MinuitParameter(const unsigned int n, MinuitParameter& par, char *parname) : 
    theNum(n), theValue(par.theValue), theError(par.theError),
    theConst(par.theConst), theFix(par.theFix), theLoLimit(par.theLoLimit), 
    theUpLimit(par.theUpLimit), theLoLimValid(par.theLoLimValid), 
    theUpLimValid(par.theUpLimValid) 
    ,theWidth(par.theWidth), theDecimals(par.theDecimals), LinkMode(par.LinkMode),
    m_MySet(par.m_MySet), MasterPtr(par.MasterPtr), MasterParameterNo(par.MasterParameterNo), m_UserIndex(n)
    {
        memcpy(theName, parname, MnPmNameLength*sizeof(char));
        par.m_UserIndex = n;    // remember index in the original copy
    }
#endif // MINUIT_EXTENDED
  

  MinuitParameter& operator=(const MinuitParameter& par) {
    theNum = par.theNum;
    memcpy(theName, par.theName, MnPmNameLength*sizeof(char));
    theValue = par.theValue;
    theError = par.theError;
    theConst = par.theConst;
    theFix = par.theFix;
    theLoLimit = par.theLoLimit; 
    theUpLimit = par.theUpLimit;
    theLoLimValid = par.theLoLimValid; 
    theUpLimValid = par.theUpLimValid;
#if MINUIT_EXTENDED
    theWidth = par.theWidth;
    theDecimals = par.theDecimals;
    LinkMode = par.LinkMode;
    m_MySet = par.m_MySet;
    MasterPtr = par.MasterPtr;
    MasterParameterNo = par.MasterParameterNo;
    m_UserIndex = par.m_UserIndex;
#endif // MINUIT_EXTENDED
   return *this;
  }

  //access methods
  unsigned int number() const {return theNum;}
  const char* name() const {return theName;}
#if MINUIT_EXTENDED

/// Return link mode corresponding to Ch
    static pmLinkMode Char2LinkMode( const char Ch);
/// Return parameter set label
    const char*    GetLabel(void) const
    {   return theName;}
  // Attribute-related functions
    /// Return parameter value
    double   GetParameterValue(bool Unlinked=false)
    {   return value(Unlinked);}
    /// Return user index
    inline int GetUserIndex(void) const {   return m_UserIndex;}
    /// Return true if the new value V is different from the cuurnt one 
    inline bool IsValueDifferentFrom(const double V) const
    {   return fabs(V-value())>fabs(V+value())*1E-6;}
    /// Set the parameter attribute 'value' 
    bool    SetParameterValue(double V, const bool Forced = false, bool Unlinked = false);

  // Formatted output related stuff
    /// Return the selected attribute formatted as string
    const std::string   GetFormattedItem(const pmAttributeIndex ItemIndex, bool linked=true, const int FillUp = 0);

    /// General purpose formatting routine
    static const std::string GetFormatted(const double val, int P, const int W=8,
                        const bool IsInteger = false, const int FillUp = 0);
    /// Return the value attribute formatted as string
    const std::string    GetFormattedValue(bool linked=true, const int FillUp = 0);
    /// Return precision parameter in the output format
    inline short    GetDecimals(void) const {   return theDecimals;}
    inline short    GetWidth(void)  const { return theWidth;}
  // Link-related functions
    /// Return the master info in string form
    const std::string  GetLinkedToString(void);
    /// Return the link mode of the record
    inline pmLinkMode GetLinkMode(void) const  {   return  LinkMode;}
    /// Return a char corresponding to the link mode
    char    GetLinkModeChar(void);
    std::string GetLinkModeString(void) {return pmLinkStrings[LinkMode];}
    /// Return the number of the slaves, linked to this master
    inline short int  GetMasterCount(void) const {   return Slaves.size();}
    /// Return parameter number in master parameterset
    inline short int    GetMasterParameterNo(void) const {   return MasterParameterNo;}
    /// Return pointer to the set the parameter belongs to
    MinuitParameterSet *GetMyParameterSet(void) const { return m_MySet;}
    /// Return pointer to the master
    inline MinuitParameterSet *GetMasterPointer(void) const {  return MasterPtr;}
    /// Set the master pointer
    inline void SetMasterPointer(MinuitParameterSet *P){MasterPtr = P;}
    /// Return true if the parameter record is linked to a master parameter record
    inline bool IsSlave(void) const
        {   return !(NULL==MasterPtr);}
    /// Return true if the pm is master for some other pm
    inline bool IsMaster(void) const
        {   return    GetMasterCount()>0;}
    /// Set the link mode given by char Ch. Return the link mode
    short   SetLinkModeByChar(const char Ch);
 // Fixing-mask related functions
    /// Return true if the parameter value is fixed
//FIXME    inline bool IsFixed(void) const
     //   {   return IsFlagSet(pmFixedValue);}
    /// Return true if the requested but is set in the flag word
//FIXME    inline bool IsFlagSet(const int NewFlag) const
       // {   return (Flags & NewFlag) == NewFlag;}
    /// Clear bits(s) in fixing mask
//FIXME    inline void ClearFlag(pmFixingMask NewFlag)
       // {   Flags = (pmFixingMask) (Flags & ~NewFlag);}
    /// Toggle bit(s) in the fixing mask
//FIXME    inline void ToggleFlag(const short int NewFlag)
    //    {   Flags = (pmFixingMask)(Flags ^ NewFlag);}
protected:
    /// Link this parameter to a parameter of a master parameter set
    bool    LinkTo(MinuitParameterSet *aMaster, const unsigned short int aMParNo, const pmLinkMode aLinkMode, MinuitParameterSet *aSlave);
    /// Unlink the parameter record from its master
    bool    UnlinkFrom(MinuitParameterSet *aSlave);
public:
    double value(bool Unlinked=false) const;
    /// Set the parameter attribute 'value' 
    void setValue(double V, const bool Forced = false, bool Unlinked = false);
	/// Return if value changed significantly in last 'setValue'
    bool	HasChanged() const {	return	m_changed;}
    /// Return sequence ID whithin the set
#else
public:
  double value() const {return theValue;}
  void setValue(double val) {theValue = val;}
#endif // MINUIT_EXTENDED

  //interaction
  double error() const {return theError;}
  void setError(double err) {theError = err;}
  void setLimits(double low, double up) {
    assert(low != up);
    theLoLimit = low; 
    theUpLimit = up;
    theLoLimValid = true; 
    theUpLimValid = true;
    if(low > up) {
      theLoLimit = up; 
      theUpLimit = low;
    }
  }

  void setUpperLimit(double up) {
    theLoLimit = 0.; 
    theUpLimit = up;
    theLoLimValid = false; 
    theUpLimValid = true;
  }

  void setLowerLimit(double low) {
    theLoLimit = low; 
    theUpLimit = 0.;
    theLoLimValid = true; 
    theUpLimValid = false;
  }

  void removeLimits() {
    theLoLimit = 0.; 
    theUpLimit = 0.;
    theLoLimValid = false; 
    theUpLimValid = false;
  }

  void fix() {theFix = true;}
  void release() {theFix = false;}
  
  //state of parameter (fixed/const/limited)
  bool isConst() const {return theConst;}
  bool isFixed() const {return theFix;}

  bool hasLimits() const {return theLoLimValid || theUpLimValid; }
  bool hasLowerLimit() const {return theLoLimValid; }
  bool hasUpperLimit() const {return theUpLimValid; }
  double lowerLimit() const {return theLoLimit;}
  double upperLimit() const {return theUpLimit;}

private:

  unsigned int theNum;
  char theName[MnPmNameLength];
  double theValue;
  double theError;
  bool theConst;
  bool theFix;
  double theLoLimit; 
  double theUpLimit;
  bool theLoLimValid; 
  bool theUpLimValid;
#if MINUIT_EXTENDED
    unsigned short int theWidth;    ///< Total width of the parameter value
    short int   theDecimals;        ///< The decimal digits after the decimal point
    pmLinkMode LinkMode;            ///< Store link mode code if MasterPtr is set
    MinuitParameterSet *m_MySet;    ///< The parameter belongs to this set
    MinuitParameterSet    *MasterPtr;  ///< Points back to the master, or NULL
    short   int MasterParameterNo;  ///< Store parameter# if MasterPtr is set
    int m_UserIndex;                ///< Index of the pm in the Minuit array
    std::vector <MinuitParameterSet*> Slaves ;      ///< Pointers to the affected slaves
	bool	m_changed;				///< If the value changed significantly in last 'set'
//  pmFixingMask    Flags;          ///< Store which of the parameters are fixed  
    friend class MinuitParameterSet;    ///< Allow to handle attributes from the set
#endif // MINUIT_EXTENDED

private:

  void setName(const char* name) {
    int l = std::min(int(strlen(name)), MnPmNameLength);
    memset(theName, 0, MnPmNameLength*sizeof(char));
    memcpy(theName, name, l*sizeof(char));
    theName[MnPmNameLength-1] = '\0';
  }
};

#if MINUIT_EXTENDED
/// The "parameter set" structure: set of parameters
class MinuitParameterSet 
{
protected:
    long        UniqueID;               ///< Component application-unique ID
    std::string    Label;              ///< The label for the parameter set
//    size_t      NoOfItems;     ///< Total number of records in this set
    bool        HasChanged;     ///< If set, at least one pm in the set changed since last use
    std::vector<int>        Affects;    ///< IDs of the attached sets, i.e. the ones which this set affects
    std::vector<int>        AffectedBy; ///< IDs of the sets affected by this one
#if 0
    wxList      *AttachmentList,///< This attached ones affect those FitParameterSet
                *AffectedList;       ///< Those FitParameterSet affect this on
#endif
    static long BaseID;     ///< The base ID for the unique IDs
    char theName[MnPmNameLength];       ///< The name of the parameter set

public:
    std::vector<MinuitParameter> Parameters; ///< The parameters belonging to this set
  // Ctor, dtor, init, etc.
    /// Create a parameter set, with aNoOfItems items and label aName
    MinuitParameterSet(char* const aName=new char);

    /// Copy constructor
    MinuitParameterSet(const MinuitParameterSet &P);

    /// Destructor
    ~MinuitParameterSet();
    /// Add a new parameter to the set
    void Add(MinuitParameter &P);
    /// Add a new parameter to the set
    void Add(MinuitParameter *P);
    /// add free parameter name, value, error
    void Add(const char* aLabel, double aValue, double aError);
    /// add limited parameter name, value, error, lower bound, upper bound
    void Add(const char* aLabel, double aValue, double aError, double aLower, double aHigher);
    /// add const parameter name, vale
    void Add(const char* aLabel, double aValue);
    /// Return total number of parameters
    inline size_t  GetCount(void) const
    {   return  Parameters.size(); }
    /// access to parameter values (row-wise)
    std::vector<double> GetParameterValues() const;
    /// Get the ParIndex-th FitParameter::GetFormattedItem
    const std::string    GetFormattedItem(const int ParIndex, const MinuitParameter::pmAttributeIndex ItemIndex)
        {return Parameters[ParIndex].GetFormattedItem(ItemIndex);}
    /// Return the ParIndex-th FitParameter::GetFormattedValue
    const std::string    GetFormattedValue(const int ParIndex)
        {return Parameters[ParIndex].GetFormattedValue();}
    /// Return parameter set label
    const char*    GetLabel(void) const
    {   return theName;}
    /// Return pointer to a parameter
    MinuitParameter *GetParameter(const unsigned int Index) 
    {
        ASSERT_MSG(Index<GetCount(), "Illegal master parameter index");
        return  &Parameters[Index];
    }
    inline const MinuitParameter & operator[](unsigned int Index) const
    {
        ASSERT_MSG(Index<Parameters.size(), "Illegal parameter index");
        return Parameters[Index];
    } 
    inline MinuitParameter & operator[](unsigned int Index)
    {
        ASSERT_MSG(Index<Parameters.size(), "Illegal parameter index");
        return Parameters[Index];
    } 
    /// Return value of the ParIndex-th parameter
    double   GetParameterValue(const int ParIndex) 
        {   return GetParameter(ParIndex)->GetParameterValue();}

    /// Link a parameter record to another parameter record
    bool    LinkParameter(const unsigned short aSParNo, const MinuitParameter::pmLinkMode aLinkMode, MinuitParameterSet *Master, const unsigned short aMParNo);
    /// Link a parameter record to another parameter record
    bool    LinkParameter(const unsigned short aSParNo, const char aLinkModeCh, MinuitParameterSet *Master, const unsigned short aMParNo)
    {   return LinkParameter(aSParNo, MinuitParameter::Char2LinkMode(aLinkModeCh), Master, aMParNo); }
    /// Unlink ParIndex-th parameter in this parameter set
    bool UnlinkParameter(const unsigned int ParIndex);

    void setName(const char* name) {
    int l = std::min(int(strlen(name)), MnPmNameLength);
    memset(theName, 0, MnPmNameLength*sizeof(char));
    memcpy(theName, name, l*sizeof(char));
    theName[10] = '\0';
  }
};
#endif
#endif //MN_MinuitParameter_H_
