#include <gtest/gtest.h>
#include "../main/include/Stuff.h"


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
    EXPECT_EQ(0 , 0 );
 }
