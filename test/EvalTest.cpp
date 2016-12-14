#include <gtest/gtest.h>
#include "Stuff.h"
#include "ESpectrumBase.h"

#include <sstream>

/** @class	StuffTest
 * @brief	Tests EMPA processor stuff functionality
 * 
 */
extern bool UNIT_TESTING;		// Switched off by default
// A new test class  of these is created for each test
class EvalTest : public testing::Test
{
public:
public:

};
 
/**
 * Tests some routines of the stuff
 */
TEST_F(EvalTest, Tools)
{
    ESpectrumBase SP;
    EXPECT_EQ(-1, SP.XEnergy_Get()); // After creating, the X energy is invalid
    EXPECT_FALSE(SP.XEnergy_Valid());
    SP.XEnergy_Set(1486.295);
    EXPECT_EQ(1486.295, SP.XEnergy_Get()); // After setting, the X energy is valid
    EXPECT_TRUE(SP.XEnergy_Valid());
}
