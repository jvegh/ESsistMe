#include <gtest/gtest.h>
#include "Stuff.h"
#include "ESpectrumBase.h"

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
TEST_F(EvalTest, Empty)
{
}
