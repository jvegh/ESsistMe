/*!  @brief General purpose data I/O, mainly for spectrum processing
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include "DataIO.h"
extern bool UNIT_TESTING;
using namespace std;


/*! \class DataIO
  \brief Represent general-purpose data I/O class
*/

// Returns true if 'the'FileName' exists and accessible
    bool DataIO::
Exists(const std::string& FileName)
{
    struct stat buffer;
    if(stat (FileName.c_str(), &buffer) == 0)
    {// File exists, check if accessible
        ifstream f(FileName.c_str());
           return f.good();
    }
    else return false;

}

    // Return next token (i.e. terminated with an item in the terminator list) from the file
    string    DataIO::
GetNextToken()
{   // Use user's terminator list, if any
    char m_CurrentLine[1024];            ///< Current line buffer
    string a;
    bool EndReached = m_FromString ?m_EndString>=m_String.size() : m_Stream->eof();
    while (m_Token.empty() && !EndReached)
        {
            if(m_FromString)
            {
                m_EndString=m_String.find_first_of("\n", m_BeginString);
                m_Token = m_String.substr(m_BeginString, (m_EndString==string::npos ? m_EndString:m_EndString-m_BeginString+1));
                m_BeginString=m_EndString+1;
            }
            else
            {
                m_Stream->getline(m_CurrentLine,1024-1);
                m_Token =  m_CurrentLine;
            }
            m_BeginToken = 0;
        }
    m_EndToken=m_Token.find_first_of(m_DefaultTerminators, m_BeginToken);
    a = m_Token.substr(m_BeginToken, (m_EndToken==string::npos ? m_EndToken:m_EndToken-m_BeginToken));
    m_BeginToken=m_EndToken+1; if(m_BeginToken>=m_Token.size()) m_Token = "";
    if(a[0]==' ')
    {   // Remove leading spaces
    }
    if(a[0] == ' ')
    {
    }
    if(a[a.size()-1] == ' ')
    {
    }
    return a;
}//DataIO::::GetNextToken


    // Read an ASCII integer from the file
    double  DataIO::
GetASCIIFloat()
{   string a = GetNextToken(); char *endptr;
    if(a.empty())
        { m_FormError = feNoArg; return 0;}
    // correct for the possible syntactical problems
    if ('.' == a[0]) a = '0'+a;
    if (('-' == a[0]) && ('.' == a[1]))
        a.insert(1,1, '0');
    double res; m_FormError = feOK;
                { res = strtod(a.c_str(),&endptr); }
    if (!(*endptr)) return res;
    m_FormError = feFloat;
        return res;
}//DataIO::GetASCIIFloat
