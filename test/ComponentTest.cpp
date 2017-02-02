#include <gtest/gtest.h>
#include "Stuff.h"
#include "EPeak1Gauss.h"
#include "ffatomki.h"


#include <sstream>

/** @class	ComponentTest
 * @brief	Tests component functionality
 * 
 */
extern bool UNIT_TESTING;		// Switched off by default
// A new test class  of these is created for each test
class ComponentTest : public testing::Test
{
public:
};
 
std::string SP1271Test("1271,1\n"
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

/**
 * Tests some routines of the stuff
 */
TEST_F(ComponentTest, Peak)
{
    // Create a spectrum first
    SpectrumESA11 IO;
    EXPECT_TRUE(IO.ReadDiskFile(SP1271Test));
    EXPECT_EQ(100,IO.NoOfDataPoints_Get());
    // Create a Gaussian
    EPeak1Gauss Gauss1(&IO, 0,  "Gauss1");
    EXPECT_EQ("Gauss1",string(Gauss1.GetLabel()));
    const double* MyVector = Gauss1.GetObjectStore();
    EXPECT_TRUE(abs(MyVector[0]) <std::numeric_limits<double>::epsilon());
    EXPECT_TRUE(abs(MyVector[IO.NoOfDataPoints_Get()-1]) <std::numeric_limits<double>::epsilon());
}

TEST_F(ComponentTest, Empty)
{
}
