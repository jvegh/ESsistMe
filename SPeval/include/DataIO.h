/*! @brief ffatomki: ATOMKI file formats for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#ifndef DATAIO_H
#define DATAIO_H
#include <string>
#include <sys/stat.h>   // For checking existence
#include <fstream>
using namespace std;
// Define form errors
enum    TextNumberErrors{ feOK, feNoArg, feFloat, feInteger};

/// 1-dim spectrum, satisfying "ESA-13" specification
class DataIO
{
public:
/*        DataIO(const std::string& FileName){
            m_DefaultTerminators = "|;, \t\r\n";
            m_Stream = new ifstream(FileName);
            m_FromString = false
        }*/
        DataIO( ifstream* Stream)
            { m_Stream = Stream; m_DefaultTerminators = "|;, \t\r\n"; m_FromString = false;}
        DataIO(const std::string& Data)
            {   m_DefaultTerminators = "|;, \t\r\n"; m_FromString = true;
                m_BeginString = 0; m_EndString = 0; m_String = Data;}

        DataIO(void){ m_DefaultTerminators = "|; \t\r\n";}
        static bool Exists(const std::string& FileName);
        virtual ~DataIO(void){}
protected:
    std::string m_Token;
    ifstream* m_Stream;
    std::string m_DefaultTerminators;
    TextNumberErrors m_FormError;
    size_t m_BeginToken, m_EndToken;
    size_t m_BeginString, m_EndString;
    std::string m_String;
    bool m_FromString;
public:
    void EmptyTokenBuffer(void){m_Token.clear();}
    /** Return error code in case of format error and clears error code
             \return code of form error  */
    short int   GetFormError(void)
       { int i = m_FormError; m_FormError = feOK; return i;}
    string GetNextToken(void);
    /**  Read an ASCII floating number from the file
         \param  stream the stream the token is taken from
         \return next float number in ASCII form from the stream     */
     double  GetASCIIFloat(void);
};// of class DataIO


#endif // DATAIO_H

