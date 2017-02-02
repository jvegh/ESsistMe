/*! @brief ffatomki: ATOMKI file formats for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#ifndef FFATOMKI_H
#define FFATOMKI_H
#include <fstream>
#include "ESpectrum1Dim.h"
/// 1-dim spectrum, satisfying "ESA-13" specification
/*
 * class SpectrumESA13 : public 	ESpectrumBase, DataIO
{public:
    /// Ctor for "ESA-13" spectrum file
        SpectrumESA13(const std::string& FileName);
    /// Copy ctor for "ESA-13" spectrum file
        SpectrumESA13(const SpectrumESA13 &S):Spectrum1Dim(S)
                {DependenceNo = S.DependenceNo;}
    /// Return a (cloned) copy of the "ESA-13" file
        virtual SpectrumBase *Clone(void) const
                {return new SpectrumESA13(*this);}
    /// Return true if the file is of form "ESA-13"
        bool FileMatchesTemplate(const wxString& path, wxClassInfo* CI);
    /// Read the spectrum from file to memory
        bool	ReadDiskFile(const wxString& path);

/ *    Function WriteDiskFile( FN : PChar ):boolean;
       virtual;
* /
};// of class SpectrumESA13
*/
/// 1-dim spectrum, satisfying "ESA-11" specification
class SpectrumESA11 : public 	ESpectrum1Dim
{
    friend class EvalTest;// Just to test private methods
public:
    /// Ctor for "ESA-11" spectrum file
        SpectrumESA11(void);
        SpectrumESA11(const std::string& FileName);
//        SpectrumESA11(const ifstream Stream);
        virtual ~SpectrumESA11(void){}
        bool FileMatchesTemplate(const std::string& Data);
        bool ReadDiskFile(const std::string& Data);
/*    /// Copy ctor for "ESA-11" spectrum file
        SpectrumESA11(const SpectrumESA11 &S):	Spectrum1Dim(S)
                {DependenceNo = S.DependenceNo;}
    /// Return a (cloned) copy of the "ESA-11" file
        virtual SpectrumBase *Clone(void) const
                { return new SpectrumESA11(*this);}
        */
};// of class SpectrumESA11

#endif // FFATOMKI_H

