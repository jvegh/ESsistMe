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

/*
struct DataPoint {
    double X;
    double Y;
    bool operator()(DataPoint D1, DataPoint D2)
      { return D1.X < D2.X; }
    bool operator()( int x, const DataPoint& d ) const
    {
       return x < MyPoints.X;
    }
};
bool operator()(const double lhs, const UINT32& rhs) {
  return (_v.at(rhs).bookCount < lhs));
}

bool compareValue(const double lhs, const double rhs)
{
    return lhs < rhs;
}
double myvals[]={10.,20.,30.,40.,50.,60.,70.};
std::vector<double> myvector (7);
std::vector<DataPoint> MyPoints(7);


//bool CompareDouble (DataPoint i,DataPoint j) {  return (i.X<j.X); }
//bool CompareDouble (int i,int j) {  return (MyPoints[i].X<MyPoints[j].X); }
bool CompareDoubles (int i,int j) {  return (myvector[i]<myvector[j]); }
// used for lower_bound, upper_bound, etc...

int IndexAt(const double E)
    {
        DataPoint M;
        M.X = E;
//        std::vector<DataPoint>::iterator MyPoint = std::lower_bound (MyPoints.begin(), MyPoints.end(), M.X, CompareDoubles);
        std::vector<DataPoint>::iterator MyPoint = std::lower_bound (MyPoints.begin(), MyPoints.end(), E );
        return MyPoint - MyPoints.begin();
    }
*/

TEST_F(EvalTest, Empty)
{
/*     std::copy ( myvals, myvals+7, myvector.begin() );
     for(int i=0; i<7; i++)
         MyPoints[i].X = myvector[i];
    std::cerr << "at 31 " << MyPoints.IndexAt(31) << std::endl;
    std::cerr << "at -20 " << MyPoints.IndexAt(-20) << std::endl;
    std::cerr << "at 500 " << MyPoints.IndexAt(500) << std::endl;
    std::cerr << "at 20 " << MyPoints.IndexAt(20) << std::endl;
*/
}

