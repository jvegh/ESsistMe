//: ctypes.h
/*  Spectrum &c constant definitions
    \author   (c) János Végh, MTA ATOMKI, Debrecen, Hungary (veghj@users.sf.net)
    \since    Nov 28, 2004
    $Id: ctypes.h,v 1.0 2004-05-03 21:19:07+02 veghj Exp veghj $
 */

#ifndef ewa_ctypesh
#define ewa_ctypesh
// This file collects all component types and subtypes, used by EWA

/// The main types of components
enum ot_ComponentTypes
{   ot_Nothing = 0,     ///< Empty component
    ot_Bckgnd,          ///< Background
    ot_Peak,            ///< Peak
    ot_Multiline,       ///< Multi-line pseudo structure
    ot_Tail,            ///< Tail pseudo structure
    ot_Convolver,       ///< Convolver pseudo structure
    ot_Shared           ///< Shared pseudo structure
};

/// The subtypes of the main components
enum  ot_ComponentSubTypes
{
//BackgroundObjectTypes
    obg_Polynom = 1,    ///< Polynomial
    obg_Shirley,        ///< Shirley-type
    obg_ShirleyCS,      ///< Shirley-type, based on cross section
    obg_BishopCS,       ///< Bishop-type, based on cross section
    obg_Tougaard,       ///< Tougaard-type, universal
    obg_ArcTan,         ///< ArcTan type
    obg_Wanier,         ///< Wannier-type
// Peak subtypes
    opk_Gauss,          ///< Gaussian
    opk_Lorentz,        ///< Lorentzian
    opk_Kuchiev,        ///< Kuchiev PCI distorted
    opk_KuchievConv,    ///< Kuchiev PCI Conv distorted
    opk_DoniachSunjic,  ///< Doniach-Sunjic type
    opk_DoniachSunjicConv,///< Doniach-Sunjic convolved type
//    opk_Cusp           
//    opk_etGauss         7
    opk_Drude1,         ///< Drude type 1
    opk_Drude2,         ///< Drude type 2
    opk_pVoigt,         ///< pseudo-Voigt
    opk_Voigt_2,        ///< 1E-2 precision Voigt
//    opk_Voigt_4,        ///< 1E-4 precision Voigt
//    opk_Voigt_6,        ///< 1E-6 precision Voigt
    opk_Voigt,          ///< 1E-10 precision Voigt
// Multiline subtypes
    oml_Vibration,      ///< Vibrational multiplet
    oml_Plasmon,        ///< Plasmon multiplet
    oml_Satellite,      ///< X-ray satellites
// Peak tail subtypes
    ott_Shirley,         ///< Shirley-type tail
    ott_Tougaard,        ///< Tougaard-type tail
    ott_ShirleyCS,       ///< Shirley-type cross section tail
    ott_IntrinsicCS,       ///< Intrinsic-type cross section tail
    ott_ExpTimesExp      ///< Tougaard-type variant tail
};

/// Constants for the polynomial background 
enum fpBgPoly
{
    fpBgPoly_ZeroShift  = 1,    ///< Zero-shift of the polynomial
    fpBgPoly_Min1storder= 2,    ///< E-1 term
    fpBgPoly_0thorder   = 4,    ///< E0 term
    fpBgPoly_1storder   = 8,    ///< E1 term
    fpBgPoly_2ndorder   = 16,   ///< E2 term
    fpBgPoly_3rdorder   = 32
};
// Constants for the inelastic background Shirley 
enum fsBgShirley
{
    fsBgShirley_Diff    =   1,  ///< The Shirley difference parameter
    fsBgShirley_Const   =   2,  ///< The constant background term
    fsBgShirley_Iter    =   4,  ///< Number of iterations
    fsBgShirley_Gap     =   8,  ///< No-interaction gap parameter
    fsBgShirley_Left    =  16,  ///< Left limit, for the BAD implementation
    fsBgShirley_Right   =  32   ///< Right limit, for the BAD implementation
};

#if ewaUSE_BACKGROUND_SHIRLEY_CS
// Constants for the cross section based inelastic background Shirley 
enum fsBgShirleyCS
{
    fsBgShirleyCS_B     =   1,  ///< The Shirley tail height parameter
    fsBgShirleyCS_C     =   2,  ///< The Shirley tail width parameter
    fsBgShirleyCS_Gap   =   4,  ///< No-interaction gap parameter
    fsBgShirleyCS_Const =   8   ///< The constant background term
};
#endif
#if ewaUSE_BACKGROUND_INTRINSIC_CS
// Constants for the cross section based inelastic background Shirley 
enum fsBgIntrinsicCS
{
    fsBgIntrinsicCS_UniB  =   1,  ///< Tougaard's "universal" B
    fsBgIntrinsicCS_UniC  =   2,  ///< Tougaard's "universal" C
    fsBgIntrinsicCS_UniD  =   4,  ///< Tougaard's "universal" D
    fsBgIntrinsicCS_B     =   8,  ///< The Shirley tail height parameter
    fsBgIntrinsicCS_C     =   16, ///< The Shirley tail width parameter
    fsBgIntrinsicCS_Gap   =   32  ///< No-interaction gap parameter
};
#endif

#if ewaUSE_BACKGROUND_BISHOP_CS
// Constants for the cross section based inelastic background Bishop 
enum fsBgBishopCS
{
    fsBgBishopCS_B      =   1,  ///< The Bishop tail height parameter
    fsBgBishopCS_C      =   2,  ///< The Bishop tail width parameter
    fsBgBishopCS_Gap    =   4,  ///< No-interaction gap parameter
    fsBgBishopCS_Const  =   8   ///< The constant background term
};
#endif

// Constants for the inelastic background_Tougaard-universal cross section
enum fsBgTougaard
{
    fsBgTougaard_UniB       =   1,  ///< Tougaard's "universal" B
    fsBgTougaard_UniC       =   2,  ///< Tougaard's "universal" C
    fsBgTougaard_UniD       =   4,  ///< Tougaard's "universal" D
    fsBgTougaard_UniConstant=   8  ///< The constant background term
};

// Constants for  background ArcTan
enum fsBgArc
{
    fsBgArc_Energy      =   1,  ///< Energy of the inflexion point
    fsBgArc_Height      =   2,  ///< The asymptotic difference
    fsBgArc_Width       =   4,  ///< The width parameter
    fsBgArc_Const       =   8   ///< The constant background term
};
//    #define obg_Wanier          7

// Constants for the general peak
enum fpPk
{
    fpPk_Position       =   0x01,  ///< Position (energy) of the peak
    fpPk_Intensity      =   0x02,  ///< Intensity (height/area) of the peak
    fpPk_Width          =   0x04,  ///< Width parameter of the peak
    fpPk_GWidth         =   0x04,  ///< Gauss width parameter of the peak
    fpPk_LWidth         =   0x08,  ///< Lorentz width parameter of the peak
    fpPk_Mixing         =   0x08,  ///< Mixing parameter for pseudo-Voigt
    fpPk_GAsymmetry     =   0x08, ///< Asymmetry parameter for Gaussian/Lorentzian
    fpPk_Alpha          =   0x08,  ///< Asymmetry (alpha) parameter for Don-Sun
    fpPk_VAsymmetry     =   0x10, ///< Asymmetry parameter for Voigt
    fpPk_Ksi            =   0x10  ///< Asymmetry (ksi) parameter for Kuchiev
};
#endif
//ewa_ctypesh
