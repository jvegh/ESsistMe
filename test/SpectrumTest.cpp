#include <gtest/gtest.h>
#include "Stuff.h"
#include "ESpectrumBase.h"

#include <sstream>

/** @class	SpectrumTest
 * @brief	Tests spectrum functionality
 * 
 */
extern bool UNIT_TESTING;		// Switched off by default
// A new test class  of these is created for each test
class SpectrumTest : public testing::Test
{
public:
};
 
/**
 * Tests some routines of the stuff
 */
TEST_F(SpectrumTest, Base_ScaleTest)
{
    double AlK = 1486.295;
    double myints[]={10.,20.,30.,40.,50.,60.,70.};
    std::vector<double> myvector (7);
    std::copy ( myints, myints+7, myvector.begin() );
    int MaxIndex = myvector.size()-1;
    ESpectrumBase SP(&myvector);
    EXPECT_EQ(-1, SP.XEnergy_Get()); // After creating, the X energy is invalid
    EXPECT_FALSE(SP.XEnergy_Valid());
    EXPECT_EQ(20,SP.Y_Get(1));
    EXPECT_EQ(1,SP.X_Get(1));
    EXPECT_EQ(1,SP.X_Get_Kinetic(1));
    EXPECT_EQ(1,SP.X_Get_Binding(1));   // BE is not yet set
    SP.XEnergy_Set(AlK);
    EXPECT_EQ(20,SP.Y_Get(1));
    EXPECT_EQ(1,SP.X_Get(1));
    EXPECT_EQ(1,SP.X_Get_Kinetic(1));
    EXPECT_EQ(AlK-1,SP.X_Get_Binding(1));
    EXPECT_EQ(AlK, SP.XEnergy_Get()); // After setting, the X energy is valid
    EXPECT_TRUE(SP.XEnergy_Valid());
    // now change the data to binding scale
    SP.Binding_Scale_Set(true); // Change the scale type (and so reverse data order)
    EXPECT_EQ(20,SP.Y_Get(MaxIndex-1));
    EXPECT_EQ(AlK-1,SP.X_Get(MaxIndex-1));
    EXPECT_EQ(1,SP.X_Get_Kinetic(MaxIndex-1));
    EXPECT_EQ(AlK-1,SP.X_Get_Binding(MaxIndex-1));
    // Now change back to kinetic scale
    SP.Binding_Scale_Set(false); // Change the scale type (and so reverse data order)
    EXPECT_EQ(20,SP.Y_Get(1));
    EXPECT_EQ(1,SP.X_Get(1));
    EXPECT_EQ(1,SP.X_Get_Kinetic(1));
    EXPECT_EQ(AlK-1,SP.X_Get_Binding(1));
    // Now change to binding scale then invalidate
    SP.Binding_Scale_Set(true); // Change the scale type (and so reverse data order)
    SP.XEnergy_Set(-1);
    EXPECT_EQ(20,SP.Y_Get(MaxIndex-1));  // Data not changed
    EXPECT_EQ(AlK-1,SP.X_Get(MaxIndex-1));
    EXPECT_EQ(AlK-1,SP.X_Get_Kinetic(MaxIndex-1));
    EXPECT_EQ(AlK-1,SP.X_Get_Binding(MaxIndex-1));
    SP.Binding_Scale_Set(false); // Change the scale type (and so reverse data order)
    EXPECT_EQ(20,SP.Y_Get(MaxIndex-1));  // Data not changed
    EXPECT_EQ(AlK-1,SP.X_Get(MaxIndex-1));
    EXPECT_EQ(AlK-1,SP.X_Get_Kinetic(MaxIndex-1));
    EXPECT_EQ(AlK-1,SP.X_Get_Binding(MaxIndex-1));
    // Now go back to the initial state
    SP.Binding_Scale_Set(true); // Change the scale type (and so reverse data order)
    SP.XEnergy_Set(AlK);
    SP.Binding_Scale_Set(false); // Change the scale type (and so reverse data order)
    SP.XEnergy_Set(-1);
    EXPECT_FALSE(SP.XEnergy_Valid());
    EXPECT_EQ(20,SP.Y_Get(1));
    EXPECT_EQ(1,SP.X_Get(1));
    EXPECT_EQ(1,SP.X_Get_Kinetic(1));
    EXPECT_EQ(1,SP.X_Get_Binding(1));   // BE is not yet set
    // Now check if the energy is found correctly
    EXPECT_EQ(3,SP.IndexOfEnergy(3.1));
    EXPECT_EQ(-1,SP.IndexOfEnergy(-2.)); // out of range
    EXPECT_EQ(-1,SP.IndexOfEnergy(50.)); // out of range
    EXPECT_EQ(0,SP.IndexOfEnergy(0.49)); // round down
    EXPECT_EQ(1,SP.IndexOfEnergy(0.51)); // round up
    EXPECT_EQ(2,SP.IndexOfEnergy(2.00)); // no rounding
    EXPECT_EQ(6,SP.IndexOfEnergy(5.99)); // Near to max
    EXPECT_EQ(6,SP.IndexOfEnergy(6.00)); // Max
    EXPECT_EQ(0,SP.IndexOfEnergy(0.00)); // Min
    // No excitation energy, so the binding and kinetic energies must be the same
    EXPECT_EQ(2,SP.IndexOfBindingEnergy(2.00)); // no rounding
    EXPECT_EQ(2,SP.IndexOfKineticEnergy(2.00)); // no rounding
    SP.XEnergy_Set(AlK);
    EXPECT_EQ(2,SP.IndexOfKineticEnergy(2.00)); // no rounding
    EXPECT_EQ(2,SP.IndexOfBindingEnergy(AlK - 2.00)); // no rounding
    SP.Binding_Scale_Set(true); // The binding scale reverses order!
    EXPECT_EQ(MaxIndex-2,SP.IndexOfKineticEnergy(2.00)); // no rounding
    EXPECT_EQ(MaxIndex-2,SP.IndexOfBindingEnergy(AlK - 2.00)); // no rounding
    EXPECT_EQ(MaxIndex-2,SP.IndexOfEnergy(AlK - 2.00));
}

std::string SP1271 = "1271,1\n"
"0,44248,8,45040,20,1,0,100\n"
"545,590,603,564,566,598,569,585,586,578\n"
"585,605,630,615,681,676,632,630,603,633\n"
"693,611,688,690,732,774,872,915,1071,1189\n"
"1386,1675,2107,2452,2970,3210,2990,2892,2256,1760\n"
"1283,1070,849,760,707,716,640,654,641,583\n"
"571,478,563,482,490,545,604,633,655,772\n"
"889,1114,1371,1687,2204,2699,3467,3937,4099,3973\n"
"3332,2739,1849,1425,883,782,562,477,390,350\n"
"293,338,313,323,301,310,359,370,417,384\n"
"350,333,306,280,265,264,253,216,296,288\n";

TEST_F(SpectrumTest, Spectrum_ESA11_Test)
{
    cerr << SP1271;
    cerr<< "This was read\n";
    string token; int found = 0; int next; int seq = 0;
    do
    {
      next = SP1271.find_first_of(" ,\n",found);
      token = SP1271.substr(found,next-found);
      found = next+1;
      cerr << seq++ << ':' << token << ' ';
    }
    while (token.size());
    cerr << endl;
 /*   while (found!=std::string::npos)
    {
      str[found]='*';
      found=str.find_first_of("aeiou",found+1);
    }


    try { // Tries to understand the contents of the file
       // throws exceptionif fails
      std::bitset<5> mybitset (std::string("01234"));
    }
    catch (const std::invalid_argument& ia) {
            qError << "Invalid argument: " << ia.what() << '\n';
    }

            char * pch;
 //           pch = strtok ((char*)SP1271.c_str()," ,|,\n"); int i = 0;
            pch = strtok (&SP1271[0]," ,|,\n"); int i = 0;
            while (pch != NULL && i++ < 10)
            {
              cerr << i << ':' << pch << ' ';
              pch = strtok (NULL, " ,|,\n");
            }
    cerr << "\nThis was interpreted\n";
    */
}

