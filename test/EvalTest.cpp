#include <gtest/gtest.h>
#include "Stuff.h"
#include "ESpectrumBase.h"
#include "DataIO.h"

#include <sstream>

/** @class	EvalTest
 * @brief	Tests evaluation stuff functionality
 * 
 */
extern bool UNIT_TESTING;		// Switched off by default
// A new test class  of these is created for each test
class EvalTest : public testing::Test
{
public:
};
 
/**
 * Tests some routines of the stuff
 */

std::string SP1271("1271,1\n"
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
                   "350,333,306,280,265,264,253,216,296,288\n");

std::string BADFORMAT(".6,-.3,\n"
                   );
TEST_F(EvalTest, Empty)
{
    // The first way is to provide a steam, like
//    ifstream* MyStream = new ifstream("/home/jvegh/DEVEL/cpp/old/wx/ewa/FFORMS/ESA_11/SP1271.DAT");
//    DataIO IO(MyStream);
    // The second way is to provide the text string directly
     DataIO IO(SP1271);
//    DataIO IO("/home/jvegh/DEVEL/cpp/old/wx/ewa/FFORMS/ESA_11/SP1271.DAT");
    EXPECT_EQ(1271,IO.GetASCIIFloat());
    EXPECT_EQ(1,IO.GetASCIIFloat());
    EXPECT_EQ(0,IO.GetASCIIFloat());
    EXPECT_EQ(44248,IO.GetASCIIFloat());
}

TEST_F(EvalTest, FormatBugs)
{
    DataIO IO(BADFORMAT);
    EXPECT_EQ(.6,IO.GetASCIIFloat());
    EXPECT_EQ(-.3,IO.GetASCIIFloat());
}
