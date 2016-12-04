#include <gtest/gtest.h>
#include <glog/logging.h>
#include "../main/include/Stuff.h"


#include <sstream>

/** @class	StuffTest
 * @brief	Tests EMPA processor stuff functionality
 * 
 */
extern bool UNIT_TESTING;		// Switched off by default
extern bool OBJECT_FILE_PRINTED;
// A new test class  of these is created for each test
class StuffTest : public testing::Test
{
public:
public:

};
 
/**
 * Tests some routines of the stuff
 */
TEST_F(StuffTest, Tools)
{
    EXPECT_EQ(0 , 0 );
 }
