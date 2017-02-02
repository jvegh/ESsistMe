/*!  @brief ATOMKI in-house spectrum formats stuff for the ESsistMe tool
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <math.h>
#include <algorithm>
#include <fstream>
#include "ffatomki.h"
extern bool UNIT_TESTING;
using namespace std;


/*!
  Spectra have multiple inheritance:
  - ESpectrumBase() defines their spectral behavior
  - DataIO() their export/import capabilities

  */
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// SpectrumESA13
////////////////////////////////////////////////////////////////////////////////////////////////////
/** ESA-13 format

  These file forms have a 10-byte heading;
  contain equidistant data, in one region only
  the uncalibrated version:
  Line #1:
  (#1) SpectrumNO,       spectrum identifier number,0-65535
  (#2) SPMIIDent         spectrometer ID,0-65535
  Line #2:
  (#3) DependenceNo,     typically unused, contains angle at ESA13, sometimes
  (#4) EnergyStart,      the first energy value
  (#5) EnergyStep,       the energy step size
  (#6) EnergyEnd,        the last energy value
  (#7) TimePerChannel,   Collection time, per channel and per cycle
                         in units 0.1sec
  (#8) Cycles,           the counts are the sum of measuring Cycles times
  (#9) NoOfChannels, >0  The number of data sets, one less
  (#10) NoOfPoints       The number of the measurement data points per channel
  Line #3:...
  measured counts, 10 value per line
*/

/*! \class SpectrumESA13
  \brief Represents 2-dim measured data in form of double-precision vectors

  The base class really stores the data, i.e. other objects can receive pointers to the data.
*/
  /*! \fn SpectrumESA13::SpectrumESA13(vector<double>* Y)
   * Single-vector constructor for the 2-dim spectrum
    Creates a base spectrum, using the intensity data only.
    The energy data are simple sequence numbers.
    The uncertainties are the square root of the intensity data
    (zero protection provided).
    The "energy" is simple a sequence number, \see Calibrate
  */
// Returns true if the file FN is of format 'ESA-13'


/*
bool	SpectrumESA13::FileMatchesTemplate(const wxString& path, wxClassInfo* CI)
{	float First10[11],aN; int Index; bool FD;
        if(Spectrum1Dim::FileMatchesTemplate(path,CI))
        { // The file exists at least }
                        ifstream stream(path);
                        EmptyTokenBuffer();
                Index = 0; //LineEmpty = true;
                FD = false; // assume failure }
                while ((Index<10) && (0==GetFormError()))
                        First10[++Index] = GetASCIIFloat(stream);
                if((Index>=10) && (0==GetFormError()))
                {	// No error encountered in the first ten values (the heading) }
                        // verify if really some ESA format }
                        if(First10[5]!=0)
                        {	aN = (int)((First10[6]-First10[4])/First10[5]);
                                if(aN < 0) aN -=1; aN = fabs(aN);
                                if( (aN<32000) && (aN>0)
                                        && (First10[10]<32000) && (First10[9]<32000) && (First10[9]>0))
                                {	NoOfEPoints = (int)First10[10];
                                        if( fabs(aN+1 - NoOfEPoints) < 0.5)
                                                { // it is uncalibrated ESA-13 format }
                                                        Calibrated = false;
                                                        NoOfDatasets = (int)First10[9]+1;
                                                        SpectrumMode = SpectrumMode || sm_MultiBlock;
                                                        FD = true;
                                                }
                                }
                        }
                }
        //	ASCIIFile.close();
    }
  else FD = false;
  return FD;
}//SpectrumESA13::FileMatchesTemplate

bool	SpectrumESA13::ReadDiskFile(const wxString& path)
{	float rgnEnd; bool OK = true;
        ifstream stream(path);
        EmptyTokenBuffer();
        SpectrumNo = GetASCIIInt(stream);
        SpectrumID = wxString::Format("%d",SpectrumNo);
        if(!GetFormError())SpectrometerID = GetASCIIInt(stream);  // spectrometer ID
        if(!GetFormError())DependenceNo  = GetASCIIFloat(stream);  //{ usually don't care }
        if(!GetFormError())GetActiveRegion()->rgnBegin = GetASCIIFloat(stream);
        if(!GetFormError())GetActiveRegion()->rgnStep = GetASCIIFloat(stream);
        if(!GetFormError())rgnEnd = GetASCIIFloat(stream);
        if(!GetFormError())GetActiveRegion()->rgnTimePerChannel = GetASCIIFloat(stream)*0.1;
        if(!GetFormError())GetActiveRegion()->rgnNoOfScans = GetASCIIInt(stream);
        if(!GetFormError())NoOfDatasets = GetASCIIInt(stream)+1;
        if(!GetFormError())GetActiveRegion()->rgnNoOfPoints = GetASCIIInt(stream);
        if(!GetFormError())OK = ReadASCIIData(stream);
        AxisE.SetScaleUnit(_("DAC step"));
        if(OK) ReadCalibrationData(path);
        return OK; // until implemented
}//SpectrumESA13::ReadDiskFile
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// SpectrumESA11
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class SpectrumESA11
  \brief Represents 2-dim measured data in form of double-precision vectors

  The base class really stores the data, i.e. other objects can receive pointers to the data.
*/
  /*! \fn SpectrumESA11::SpectrumESA11(vector<double>* Y)
  */

    SpectrumESA11::
SpectrumESA11(void) :  ESpectrum1Dim()//, DataIO()
{
}
    SpectrumESA11::
SpectrumESA11(const std::string& FileName) :
        ESpectrum1Dim(FileName)//,
   //     DataIO(FileName)
{
}
    bool SpectrumESA11::
FileMatchesTemplate(const std::string& Data)
{
        int First10[11],aN; int Index;
        m_String = Data; m_FormError = feOK; m_BeginString = 0; m_EndString = 0;
        EmptyTokenBuffer();
        Index = 0;
        // Now attempt to read the 10-value heading
        while ((Index<10) && (0==GetFormError()))
                    First10[++Index] = GetASCIIInt();
        if((Index<10) || (0!=GetFormError()))
            return 0;
        // The heading does not exclude it can be ESA11 format
         // verify if really some ESA11 format
        if((First10[1]<0) || (First10[1]>0xFFFF)) return 0; // Illegal spectrum number
        if(First10[3]>0) return 0;  // Different, like ESA13
        if((First10[6]<=First10[4]) && (First10[5]>0)) return 0; // End is less then begin
        int NoOfEPoints = (int)First10[10]; // No of points, intended
        aN = (int)((First10[6]-First10[4])/First10[5])+1; // No of points, actual
        if(aN!=NoOfEPoints) return false; // Number of points is bad
        if(aN>3200) return false; // Too many points
        if(First10[9] > 0) return false; // More than one channels
        if(First10[10] > 3200) return false; // Bad DAC address
        // Now check spectral data: if there are thr promised number of good points
        Index = 0;
        int Error = 0;
        do {
             GetASCIIInt(); //Just count them
            Error = GetFormError();
            if(!Error)
               ++Index;
        } while ( !Error);
        if(Index==NoOfEPoints) return true;
        return false;
}
    /** ESA-11 format

      These file forms have a 10-byte heading;
      contain equidistant data, in one region only
      the uncalibrated version:
      Line #1:
      (#1) SpectrumNO,       spectrum identifier number,0-65535
      (#2) SPMIIDent         spectrometer ID,0-65535
      Line #2:
      (#3) DependenceNo,     typically unused, contains angle at ESA13, sometimes
      (#4) EnergyStart,      the first energy value, DAC
      (#5) EnergyStep,       the energy step size, DAC
      (#6) EnergyEnd,        the last energy value, DAC
      (#7) TimePerChannel,   Collection time, per channel and per cycle
                             in units 0.1sec
      (#8) Cycles,           the counts are the sum of measuring Cycles times
      (#9) NoOfChannels, =0  The number of data sets, one less
      (#10) NoOfPoints       The number of the measurement data points per channel
      Line #3:...
      measured counts, 10 value per line
    */
/*!
    \brief Import (read from storage) a spectrum stored in textual form

*/
    bool SpectrumESA11::
ReadDiskFile(const std::string& Data)
{
        if(!FileMatchesTemplate(Data)) return false;    // Refuse reading wrong format
        int First10[11]; int Index;
        m_String = Data; m_FormError = feOK; m_BeginString = 0; m_EndString = 0;
        EmptyTokenBuffer();
        Index = 0;
        // Now  read the 10-value heading
        while ((Index<10) && (0==GetFormError()))
                    First10[++Index] = GetASCIIInt();
        if((Index<10) || (0!=GetFormError()))
            return 0;
        // At this point the format is surely correct and the heading is in First10

        mData.resize(First10[10]); double x = First10[4]; double y;
        for (std::vector<ESpectrumPoint>::iterator Dit = mData.begin(); Dit!=mData.end(); ++Dit)
        {
          y = GetASCIIInt();
          Dit->Y = y; Dit->X = x; x += First10[5];  // Use a sequence number as "energy"
          Dit->dYR = y >= 1 ? sqrt(y) : 1;   // Protect from zero uncertainty
          Dit->dYR = 1/Dit->dYR;  // For efficiency, stores 1/dY
          Dit->Fit = 0.L;       // Initialize fitted value to 0
        }
        return true;
} //
/*
bool	SpectrumESA11::ReadDiskFile(const wxString& path)
{	float rgnEnd; bool OK=true;
        ifstream stream(path);
        EmptyTokenBuffer();
        SpectrumNo = GetASCIIInt(stream);
        SpectrumID << SpectrumNo;	// spectrum identifier number
        if(!GetFormError())	SpectrometerID	= GetASCIIInt(stream);	// spectrometer ID
        if(!GetFormError())DependenceNo	= GetASCIIFloat(stream);//{ usually don't care }
        if(!GetFormError())GetActiveRegion()->rgnBegin = GetASCIIFloat(stream);
        if(!GetFormError())GetActiveRegion()->rgnStep = GetASCIIFloat(stream);
        if(!GetFormError())rgnEnd = GetASCIIFloat(stream);
        if(!GetFormError())GetActiveRegion()->rgnTimePerChannel = GetASCIIFloat(stream)*0.1;
        if(!GetFormError())GetActiveRegion()->rgnNoOfScans = GetASCIIInt(stream);
        if(!GetFormError())NoOfDatasets = GetASCIIInt(stream)+1;
        if(!GetFormError())GetActiveRegion()->rgnNoOfPoints = GetASCIIInt(stream);
        if(!GetFormError())	OK	= ReadASCIIData(stream);
        AxisE.SetScaleUnit(_("DAC step"));
        if(OK)	OK = ReadCalibrationData(path);
        return OK;
}//SpectrumESA11::ReadDiskFile
*/
