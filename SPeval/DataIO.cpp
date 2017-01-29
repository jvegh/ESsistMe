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
// Returns the stream to file or null if fails
    string DataIO::
String_Get(const std::string& FileName)
{
    if(Exists(FileName))
    {//http://www.cplusplus.com/forum/general/109435/
        std::ifstream in(FileName);
        std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        s.shrink_to_fit();
        return s;
    }
    else
        return "";
}

    // Return next token (i.e. terminated with an item in the terminator list) from the file
    string    DataIO::
GetNextToken()
{   // Use user's terminator list, if any
    while (m_Token.empty() && (m_EndString<m_String.size()))
        {
            m_EndString = m_String.find("\r\n", m_BeginString);
            if(string::npos == m_EndString )
            {  // we have a "\n" line termination only
                m_EndString = m_String.find("\n", m_BeginString);
                m_Token = m_String.substr(m_BeginString, (m_EndString==string::npos ? m_String.size():m_EndString-m_BeginString+1));
                m_BeginString=m_EndString+1;
            }
            else
            { // We have a "\r\n" line terniation
               // m_EndString=m_String.find("\r\n", m_BeginString);
                m_Token = m_String.substr(m_BeginString, (m_EndString==string::npos ? m_String.size():m_EndString-m_BeginString)) + '\n';
                m_BeginString=m_EndString+2;
            }
            m_BeginToken = 0;
        }
    m_EndToken=m_Token.find_first_of(m_DefaultTerminators, m_BeginToken);
    string a = m_Token.substr(m_BeginToken, (m_EndToken==string::npos ? m_EndToken:m_EndToken-m_BeginToken));
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



    // Read an ASCII integer from the stream
    long int    DataIO::
GetASCIIInt()
{   string a = GetNextToken(); char *endptr;
    long int res; m_FormError = feOK;
    if(a.empty())
        { m_FormError = feNoArg; return 0;}
    res = strtol(a.c_str(),&endptr,10);
    if (!(*endptr)) return res;
    m_FormError = feInteger;
    return res;
}


    // Read an ASCII float number from the stream
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
    res = strtod(a.c_str(),&endptr);
    if (!(*endptr)) return res;
    m_FormError = feFloat;
        return res;
}

   string  DataIO::
GetASCIIString(int length)
   {
       return "";
   }


/*
   //get string of max length 'length'
   wxString    SpectrumDocument::GetASCIIString(istream& stream, int length)
   {   wxString a;
       unsigned int    maxlength = length;
       while (!m_tokenizer.HasMoreTokens() && !stream.eof())
           {   stream.getline(m_CurrentLine,sizeof(m_CurrentLine)-1);
               m_tokenizer.SetString(m_CurrentLine,m_DefaultTerminators);
           }
       a = m_tokenizer.GetString();
       if((a.Len()<maxlength) || (maxlength==0)) maxlength = a.Len();
       m_tokenizer.SetString(a.Mid((size_t) maxlength));
       return a.Left((size_t) maxlength);
   }//SpectrumDocument::GetNextString
 */
