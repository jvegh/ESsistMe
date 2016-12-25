#include "MinuitParameter.h"
#if MINUIT_EXTENDED

#include    <algorithm>
#include    <cassert>
#include    <iostream>

std::ostream& operator << (std::ostream &oss, const MinuitParameter &t) {
    oss << t.theNum << ' ';
    oss << t.theName << ' ';
    oss << t.theValue << ' ';
    oss << t.theError << ' ';
    oss << t.theConst << ' ';
    oss << t.theFix << ' ';
    oss << t.theLoLimit << ' ';
    oss << t.theUpLimit << ' ';
    oss << t.theLoLimValid << ' ';
    oss << t.theUpLimValid << ' ';
#if MINUIT_EXTENDED
    oss << t.theWidth << ' ';
    oss << t.theDecimals << ' ';
//    oss << t.LinkMode << ' ';
//    m_MySet = par.m_MySet;
//    MasterPtr = par.MasterPtr;
//    oss << t.MasterParameterNo;
    oss << t.m_UserIndex << ' ';
#endif // MINUIT_EXTENDED
    return oss;
}

std::istream& operator>> (std::istream &in, MinuitParameter &t)
{
    in >> t.theNum;
    in >>  t.theName;
    in >>  t.theValue;
    in >>  t.theError;
    in >>  t.theConst;
    in >>  t.theFix;
    in >>  t.theLoLimit;
    in >>  t.theUpLimit;
    in >>  t.theLoLimValid;
    in >>  t.theUpLimValid;
#if MINUIT_EXTENDED
    in >>  t.theWidth;
    in >>  t.theDecimals;
//    in >>  t.LinkMode;
//    m_MySet = par.m_MySet;
//    MasterPtr = par.MasterPtr;
//    oss << t.MasterParameterNo;
    in >>  t.m_UserIndex ;
#endif // MINUIT_EXTENDED
     return in;
}

std::ostream& operator << (std::ostream &oss, const MinuitParameterSet &t) {
    oss << t.Parameters.size() << ' ';
    oss << t.theName << ' ';
    oss << t.Label << std::endl;
    for(unsigned int i = 0; i< t.Parameters.size(); i++)
        oss << t.Parameters[i] << std::endl;
/*    oss << t.theNum << ' ';
    oss << t.theName << ' ';
    oss << t.theValue << ' ';
    oss << t.theError << ' ';
    oss << t.theConst << ' ';
    oss << t.theFix << ' ';
    oss << t.theLoLimit << ' ';
    oss << t.theUpLimit << ' ';
    oss << t.theLoLimValid << ' ';
    oss << t.theUpLimValid << ' ';
#if MINUIT_EXTENDED
    oss << t.theWidth << ' ';
    oss << t.theDecimals << ' ';
//    oss << t.LinkMode << ' ';
//    m_MySet = par.m_MySet;
//    MasterPtr = par.MasterPtr;
//    oss << t.MasterParameterNo;
    oss << t.m_UserIndex << ' ';
#endif // MINUIT_EXTENDED
    */
    return oss;
}

std::istream& operator>> (std::istream &in, MinuitParameterSet &t)
{
/*    in >> t.theNum;
    in >>  t.theName;
    in >>  t.theValue;
    in >>  t.theError;
    in >>  t.theConst;
    in >>  t.theFix;
    in >>  t.theLoLimit;
    in >>  t.theUpLimit;
    in >>  t.theLoLimValid;
    in >>  t.theUpLimValid;
#if MINUIT_EXTENDED
    in >>  t.theWidth;
    in >>  t.theDecimals;
//    in >>  t.LinkMode;
//    m_MySet = par.m_MySet;
//    MasterPtr = par.MasterPtr;
//    oss << t.MasterParameterNo;
    in >>  t.m_UserIndex ;
#endif // MINUIT_EXTENDED
*/
     return in;
}

// Return link mode corresponding to Ch
MinuitParameter::pmLinkMode MinuitParameter::Char2LinkMode( const char Ch)
{   switch(Ch)
    {
        case pmReplaceLinkChar:
            return pmReplaceLink; 
        case pmAddLinkChar: 
            return pmAddLink; 
        case pmMultiplyLinkChar:
            return pmMultiplyLink; 
        case pm2LinearLinkChar: 
            return pm2LinearLink;
        case pm2FWHMLinearLinkChar:
            return pm2FWHMLinearLink;
        default:   
            return pmNoLink;
    }
}

/**  General purpose formatting, mainly for MinuitParameter::GetFormatted
    \param  val the value to be formatted
    \param  P   the precision in the formatted string ( -1: scientific notation)
    \param  W   the width in the formatted string
    \param  IsInteger   if true, the integer format forced
    \param  FillUp if to fill up to field width: 0 no, 1 right, -1 left
*/
const std::string MinuitParameter::GetFormatted(const double val, int P, const int W, const bool IsInteger, const int FillUp)
{
    char *cp,*cps,c,cs[256];
    int Width,Precision;
    Width = W; Precision = P; bool Scientific = false;
    if((P>=0)||IsInteger)   //definite precision
    {   if(fabs(val)>0) P = (int)(log10(fabs(val)) +1.999);
        else P = 2;
        if(val<0) P += 1;       // reserve space for the sign
        if((P>W-1)&&!((0==Precision) || (W>=P) || (IsInteger)))
        {   P = -1;}
        else
        {   P = W - P; if(P<0) P = 0;
            if((P>Precision)&&(Precision>=0)) P = Precision;
            if((P==0) || IsInteger) P=0;
        }
    }
    if((P<0)&&!IsInteger) //scientific format
    {   P = Width-5; if(val<0) P -= 1;
        if(val!=0) if(fabs(log10(fabs(val)))>9) P -= 1;
        if(log10(fabs(val))>=0) P += 1;
        if(P<0) P =0;
        Scientific = true;
    }
//    outs.setf(ios::uppercase);
    if(Scientific)
        sprintf(cs,"%*.*E",W,P,val);
    else if(0 == P)
        sprintf(cs,"%*ld",W,(long)val);
    else
        sprintf(cs,"%*.*f",W,P,val);
    cp = strchr(cs,'E');
    if(cp)
    { //we have an exponent, process it
        char *cp1= strchr(cs,'.');
        if (cp1<cp)
        {// we have a x.xxEyy number, check if x.xx0Eyy
            cps = cp-1;
            while(*(cps)=='0'/*&&!(*(cps+1)==0)*/)
            {   cp = cps--;
                while(*cp)  { c = *(cp+1); *cp = c; cp++;}
            }
            if((*(cps)=='.') && (*(cps+1)=='E'))
            {// we have a x.Eyy number, remove '.'
                cp = cps/*--*/;
                while(*cp)  { c = *(cp+1); *cp = c; cp++;}
            }
        }

        cp = strchr(cs,'E')+2; cps = cp;
        while((*cps=='0')&&!(*(cps+1)==0)) // we have unneeded '0' after 'E'
        {   cp = cps;
            while(*cp)  { c = *(cp+1); *cp = c; cp++;}
        }; // remove it
        //Check for redundant exponent digits
        cp = cps-2;
        if ((*cps=='0')&&(*cp=='E'))// we have E+0 or E-0, remove it
        while(*cp) { c = *(cp+3); *cp = c; cp++;}; // remove it
        cp = strchr(cs,'E');
        //Check for redundant exponent sign
        if (cp)
        { cp++; if((*cp=='+'))
          while(*cp) { c = *(cp+1); *cp = c; cp++;}
        }
    }//End of exponent processing
    if(FillUp)
    {   // Add filling spaces
        while((int)strlen(cs)<Width)
            if(FillUp<0)
            {
                char ch1[256];
                strcpy(ch1," "); strcat(ch1, cs); strcpy(cs,ch1);
            }
            else
            {   
                strcat(cs," ");
            }
    }
    else
    while(' ' == cs[0])
        strcpy(cs,&cs[1]);
    return std::string(cs);
}

/** Returns the string corresponding to the given item
    \param  ItemIndex   the selected attribute
    \param  linked      if to format the linked value
    \return the string form of the attribute 
    \param  FillUp if to fill up to field width: 0 no, 1 right, -1 left
    \see GetFormatted
 */
const std::string MinuitParameter::GetFormattedItem(const pmAttributeIndex ItemIndex, bool linked, const int FillUp)
{   int W,P;
    double val=0.0;
    P = (linked && (pmMultiplyLink==GetLinkMode()) )? -1 : GetDecimals();
    W = (0==GetWidth() || ((linked && (pmMultiplyLink==GetLinkMode())) )) ? 8 : GetWidth();
    // Get the corresponding attribute
    switch(ItemIndex)
    {   case pmLimLow  : val = theLoLimit; break;
        case pmValue : val = GetParameterValue(!linked); break;
        case pmLimHigh  : val = theUpLimit; break;
        case pmError : val = theError; break;
        case pmWidth : val = theWidth; break;
        case pmPrecision:val=theDecimals; break;
        case pmIntegerForm : break; // Just for the GNU compiler
    }// of switch ItemIndex
    if(ItemIndex>=pmWidth) W = 2;
    if( (ItemIndex==pmError) )
    { if (P>=0) 
        { if (W<P+5) W = P+5; }
        P = -1;
    }
    return GetFormatted(val,P,W,/*(Flags & pmIntegerOnly) ||*/ (ItemIndex >= pmWidth), FillUp);
}

/*!  Return the value attribute formatted as string
     \param  linked      if to format the linked value
     \param FillUp if to fill up to field width: 0 no, 1 right, -1 left
     \return formatted value 
 */
const std::string    MinuitParameter::GetFormattedValue(bool linked, const int FillUp)
{   return GetFormattedItem(pmValue, linked, FillUp);}

/// Return the character representation of the linking modes
char MinuitParameter::GetLinkModeChar(void) 
{   unsigned int lm = 0;
    int pmLinkModes[] = { pmNoLink, pmReplaceLink, pmAddLink, pmMultiplyLink,
        pm2LinearLink, pm2FWHMLinearLink};
    char pmLinkChars[] = {pmNoLinkChar, pmReplaceLinkChar, pmAddLinkChar, pmMultiplyLinkChar,
                        pm2LinearLinkChar, pm2FWHMLinearLinkChar}; 
    std::string pmLinkStrings[] = {"No link", "Replace", "Add", "Multiply", 
                        "Linear", "FWHM L", "Unknown"};
    for(lm = 0; lm < sizeof(pmLinkModes); lm ++)
        if( pmLinkModes[lm] == LinkMode) break;
    return pmLinkChars[lm ];
}


/** Links the parameter to its master
    \param  aMaster the master parameter
    \param  aMParNo param seq# in the master parameter set
    \param  aLinkMode   the link mode
    \param  aSlave  the slave parameter set
    \return true if linking successful, false otherwise
*/
bool MinuitParameter::LinkTo(MinuitParameterSet *aMaster, const unsigned short int aMParNo, const pmLinkMode aLinkMode,
                             MinuitParameterSet *aSlave)
{   
    ASSERT_MSG(aMaster, "Illegal master parameter");
    ASSERT_MSG(aMParNo < aMaster->GetCount(), "Illegal master parameter index");
    ASSERT_MSG( (aLinkMode & pm2ParLinkMask) ||
            (!(aLinkMode & pm2ParLinkMask) && (aLinkMode <= pm1LinkHigh)), "Illegal link mode"); 
    ASSERT_MSG( !(aLinkMode & pm2ParLinkMask) ||
        ((aLinkMode & pm2ParLinkMask) && (aLinkMode <= pm2LinkHigh)), "Illegal link mode"); 
    ASSERT_MSG( !(aLinkMode & pm2ParLinkMask) ||
            ((aLinkMode & pm2ParLinkMask) && (aMParNo<aMaster->GetCount()-1)), "Illegal 2-pm master parameter index");

    MinuitParameter *pr, *pr1;
    double LinkedV=0.0,V=0.0;
    bool goodlink = (pmNoLink==LinkMode)       // Parameter is not linked
                    && !MasterPtr
                    && ((*aMaster).GetCount()>=aMParNo);// Master parameter exists
    if(goodlink)
    {   
        V = GetParameterValue();    // Save value before linking
        MasterParameterNo = aMParNo; //FIXME Flags = (pmFixingMask) (Flags | pmFixedValue); // Fix the slave's value
        fix();  // Fix this (linked) value
        MasterPtr = aMaster; LinkMode = aLinkMode;
        if(MasterPtr)
            {
                pr = &((*MasterPtr).Parameters[MasterParameterNo]);
                LinkedV = (*pr).GetParameterValue();
                (*pr).Slaves.push_back(this->GetMyParameterSet());
//FIXME                (*pr).MasterCount += 1;
            }
        else LinkMode = pmNoLink;
//FIXME        aSlave->SubtractOldData();  // Remove comp before linking
  double LinkedS;
        switch(LinkMode)
        {   case pmNoLink:  // restore 'not linked' status
                theValue = V;
                MasterPtr = NULL;
                break;
            case pmReplaceLink : // Establish replace mode, preserve value
                break;
            case pmAddLink :        // Establish add mode
                theValue = V - LinkedV;
                break;
            case pmMultiplyLink :   // Establish multiply mode
                if(fabs(LinkedV)>1E-10)
                    theValue =  V/LinkedV;
                else theValue = 0;
                break;
            case pm2FWHMLinearLink: // Establish linear FWHM mode, preserve value
                break;
            case pm2LinearLink: // Establish linear mode 
                pr1 = &((*MasterPtr).Parameters[MasterParameterNo+1]);
                LinkedS = (*pr1).GetParameterValue();
                if(fabs(LinkedS)>1E-10)
                    theValue = (V - LinkedV)/LinkedS;
                else
                    theValue = 0;
                break;
           default: ;
        }
    std::vector <MinuitParameterSet*>::iterator Iter;
//    std::vector <MinuitParameterSet*> &SL = MasterPtr->Parameters[MasterParameterNo].Slaves;
#if 0
    std::cout << std::showbase << std::hex << "Having slaves L = ( " ;
    for ( Iter = SL.begin( ) ; Iter != SL.end( ) ; Iter++ )
    {
        std::cout << (long)*Iter << " ";
    }
    std::cout << ")" << std::dec << std::endl;
#endif //0
#if 0
        aSlave->AddNewData();
        // Now linking done; put slave on the Affects list
        if(aMaster && (aSlave!=aMaster))
            aMaster->AffectedListAppend(aSlave->GetUniqueID());
#endif
    }
    return goodlink;
}

/** Unlinks this parameter from its master
    \param  aSlave  the slave parameter set 
*/
bool MinuitParameter::UnlinkFrom(MinuitParameterSet *aSlave)
{
//    bool goodlink;
    double LinkedV, LinkedS;
    CHECK2_MSG(MasterPtr, return false, "Master pointer is NULL")
    MinuitParameter *pr = &(*MasterPtr).Parameters[MasterParameterNo];
    CHECK2_MSG(pr->GetMasterCount()>0, return false, "Master count <=0")
#if 0
    // Now it looks like we can do the unlinking:
    MasterPtr->AffectedListDelete(aSlave->GetUniqueID());   // Master will not affect any more
#endif
    release();  // Release the formerly fixed value

    // Look if we have the value in the list
    std::vector <MinuitParameterSet*>::iterator result;
    std::vector <MinuitParameterSet*>::iterator Iter;
    std::vector <MinuitParameterSet*> &SL = MasterPtr->Parameters[MasterParameterNo].Slaves;
#if 0
    std::cout << "Looking for " << (long)aSlave  << std::endl;
    std::cout << "My set is " << (long)GetMyParameterSet()  << std::endl;
    std::cout << std::showbase << std::hex << "Having slaves L = ( " ;
    for ( Iter = SL.begin( ) ; Iter != SL.end( ) ; Iter++ )
        std::cout << (long)*Iter << " ";
    std::cout << ")" << std::dec << std::endl;
#endif //0
    result = find( SL.begin( ), SL.end( ), aSlave );
    if  ( result == SL.end( ) )
        std::cout << "Element not found." << std::endl;
   else
   {
        SL.erase (result);
#if 0
        std::cout << "Deleting "<< std::showbase << std::hex  << (long)*result << " " << std::endl;
        std::cout << "Having slaves L = ( " ;
        for ( Iter = SL.begin( ) ; Iter != SL.end( ) ; Iter++ )
           std::cout << (long)*Iter << " ";
        std::cout << ")" << std::dec << std::endl;
#endif //0
   }


    LinkedV = pr->GetParameterValue();// Get linked value and restore the original one
    switch(LinkMode)
    {
        case pmReplaceLink : // undo replace mode; the original value stored
                break;
        case pmAddLink :  // Undo add mode
                theValue += LinkedV;
                break;
        case pmMultiplyLink :     // Undo multiply mode
                theValue *=  LinkedV;
                break;
        case pm2LinearLink: // Undo linear mode
                LinkedS = (*MasterPtr).Parameters[MasterParameterNo+1].GetParameterValue();// Get linked slope
                theValue = LinkedV + theValue * LinkedS;
                break;
        case pm2FWHMLinearLink:// Undo linear FWHM link; use saved original
                break;
        default: ;
    }
    MasterPtr = NULL;   // Be sure it is not linked
    LinkMode = pmNoLink;
    return true;
}



long MinuitParameterSet::BaseID = 0;     // Initialize the ID


/*
 * Methods for MinuitParameterSet
 */

/*! Reserve space for aNoOfItems parameters; It is the default constructor, too
    \param  aNoOfItems  the number of items to create
    \param  aName   the label for the _set_
    \see MinuitParameter
 */
MinuitParameterSet::MinuitParameterSet( char* aName)
{   
    setName(aName);
    HasChanged = true;
#if 0
    if(aNoOfItems)
        Parameters = new MinuitParameter[NoOfItems];
    else
        Parameters = (MinuitParameter*)NULL;  // The default constructor
    AttachmentList = new wxList;
    AffectedList = new wxList;
#endif
}

// MinuitParameterSet copy constructor
MinuitParameterSet::MinuitParameterSet(const MinuitParameterSet &T)
{   
    Label = T.Label; 
    HasChanged = T.HasChanged; 
#if 0
    NoOfItems = T.NoOfItems;
    AttachmentList = new wxList;
    AffectedList = new wxList;
    for(i=0; i<T.AttachmentList->GetCount(); i++)
        AttachmentList->Append((wxObject*)T.AttachmentList->Item(i)->GetData());
    for(i=0; i<T.AffectedList->GetCount(); i++)
        AffectedList->Append((wxObject*)T.AffectedList->Item(i)->GetData());
    if(T.Parameters)
        {   
            Parameters = new FitParameter[NoOfItems];    // Copy the structure
            for(size_t i = 0; i < NoOfItems; i++)
                {   
                    Parameters[i] = T.Parameters[i];
                    Parameters[i].SetLabel( T.Parameters[i].GetLabel());
                }
        }
    else Parameters = (FitParameter*)NULL;
#endif
}//MinuitParameterSet::MinuitParameterSet

/*! Add an existing parameter to the set
    \param  P   The parameter to add
 */
void MinuitParameterSet::Add(MinuitParameter &P)
{
    P.m_MySet = this;
    Parameters.push_back(P);
}// MinuitParameterSet::Add(const MinuitParameter &P)

/*! Add an existing parameter to the set
    \param  P   The parameter to add
 */
void MinuitParameterSet::Add(MinuitParameter *P)
{
    P->m_MySet = this;
    Parameters.push_back(*P);
}// MinuitParameterSet::Add(const MinuitParameter &P)

/// add free parameter name, value, error
void MinuitParameterSet::Add(const char* aLabel, double aValue, double aError)
{
    int num = 1;
    Add(new MinuitParameter(num, aLabel, aValue, aError));
}

/// add limited parameter name, value, error, lower bound, upper bound
void MinuitParameterSet::Add(const char* aLabel, double aValue, double aError, double aLower, double aHigher)
{
    int num = 1;
    Add(new MinuitParameter(num, aLabel, aValue, aError, aLower, aHigher));
}

/// add const parameter name, value
void MinuitParameterSet::Add(const char* aLabel, double aValue)
{
    int num = 1;
    Add(new MinuitParameter(num, aLabel, aValue));
}

// Dispose NoOfItems parameters
MinuitParameterSet::~MinuitParameterSet()
{
    Parameters.clear();
//    Parameters.swap(std::vector<MinuitParameter> (Parameters));
    Parameters.swap(Parameters);
}


std::vector<double> MinuitParameterSet::GetParameterValues() const 
{
  std::vector<double> result; result.reserve(Parameters.size());
  for(std::vector<MinuitParameter>::const_iterator ipar = Parameters.begin();
      ipar != Parameters.end(); ipar++)
    result.push_back((*ipar).value());

  return result;
}

/*! Link a parameter record to another parameter record
    \param  aSParNo index of the slave parameter in this set
    \param  aLinkMode   The link mode 
    \param  Master  The master parameter set
    \param  aMParNo The index of the master parameter in its set
	\return	if the link was successful
    */
bool    MinuitParameterSet::LinkParameter(const unsigned short aSParNo, const MinuitParameter::pmLinkMode aLinkMode, MinuitParameterSet *Master, const unsigned short aMParNo)
{
    ASSERT_MSG(aSParNo<GetCount(), "Illegal slave parameter index");
    return Parameters[aSParNo].LinkTo(Master,aMParNo,aLinkMode,this); 
}

/*! Unlink ParIndex-th parameter in this parameter set
    \param  ParIndex index of the slave parameter in this set
	\return	if the unlink was successful
 */
bool MinuitParameterSet::UnlinkParameter(const unsigned int ParIndex)
{
    CHECK2_MSG(ParIndex < GetCount(), return false, "Illegal slave parameter index");
    MinuitParameter *slave = &Parameters[ParIndex];
    return slave->UnlinkFrom(this);
}

/*!
	Return the (maybe linked) value of parameter
	\param	
 */
double MinuitParameter::value(bool Unlinked) const
{
    if((NULL==MasterPtr) || Unlinked)
        return theValue;
    else
    {	double MasterValue = MasterPtr->Parameters[MasterParameterNo].value();
		switch(LinkMode)
            {   case pmNoLink:
                    return theValue;// break;
                case pmReplaceLink:
                    return MasterValue;// break;
                case pmAddLink:
                    return (theValue + MasterValue);//  break;
                case pmMultiplyLink:
                    return (theValue * MasterValue);// break;
                case pm2LinearLink:
                    return MasterValue
                        + MasterPtr->Parameters[MasterParameterNo+1].value() * theValue;
				case pm2FWHMLinearLink:
					return MasterValue
                        + MasterPtr->Parameters[MasterParameterNo+1].value() * m_MySet->Parameters[0].value();
					return 0;
		default: ;
            }
    }
    return 0;	// dummy return
}

/*!
	Set the (maybe linked) value of the parameter from val
	 */
void MinuitParameter::setValue(double V, const bool Forced, bool Unlinked)
{
	double vv,vv1; 
    if(!Forced)
        {
			if(hasLowerLimit() && (V<theLoLimit)) V = theLoLimit;
            if(hasUpperLimit() && (V>theUpLimit)) V = theUpLimit;
        }   
    if(MasterPtr && !Unlinked)
        {   vv = (*MasterPtr).Parameters[MasterParameterNo].value();
            switch(LinkMode )
            {   case pmReplaceLink:
                    V = vv; break;
                case pmAddLink:
                    V = V - vv; break;
                case pmMultiplyLink:
                    if(fabs(vv)>1E-10) V = V/vv;
                else 
                    V =0; break;
                case pm2LinearLink:
                    vv1 = (*MasterPtr).Parameters[MasterParameterNo+1].value();
                    if(fabs(vv1)>1E-10) 
                        V = (V - vv)/vv1;
                    else 
                        V =0; break;
                    break;
				case pm2FWHMLinearLink:
                    vv1 = (*MasterPtr).Parameters[MasterParameterNo+1].value();
					ASSERT_MSG(0,"not yet tested");
                    if(fabs(vv1)>1E-10) 
                        V = (V - vv)/vv1;
                    else 
                        V =0; break;
                    break;
		default: ;
           }
        }
    m_changed = IsValueDifferentFrom(V); theValue = V; 
}

#endif  // MINUIT_EXTENDED
