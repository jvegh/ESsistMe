#ifndef MN_LinkTest_H_
#define MN_LinkTest_H_
#include <gtest/gtest.h>
#include <glog/logging.h>

class MinuitLinkTest : public testing::Test
{
/*  CPPUNIT_TEST_SUITE( MinuitLinkTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testLinking );
  CPPUNIT_TEST( testExtract );
  CPPUNIT_TEST_SUITE_END();
*/
public:

virtual void 
setUp()
{
  std::cerr<< "Setup running";
/*  GaussE=100.0;
  GaussY=1000;
  GaussW=5.0;
  GaussA=0.1;
	Gaussian = new MinuitParameterSet((char*)std::string("Gaussian").c_str());
	Gaussian->Add("Energy",GaussE, GaussE/1000);
	(*Gaussian)[0].setLimits(GaussE/2,GaussE*2);
	Gaussian->Add("Yield",GaussY, GaussY/1000);
	(*Gaussian)[1].fix();
	Gaussian->Add("Width",GaussW, GaussW/500);
	(*Gaussian)[2].setUpperLimit(GaussW*3);
	Gaussian->Add("Alpha",GaussA);
  LorentzE=104.0;
  LorentzY=2000.0;
  LorentzW=5.0;
  LorentzA=0.2;
	Lorentzian = new MinuitParameterSet((char*)std::string("Lorentzian").c_str());
	Lorentzian->Add("Energy",LorentzE, 0.1);
	(*Lorentzian)[0].setLimits(50,200);
	Lorentzian->Add("Yield",LorentzY, 1.1);
	Lorentzian->Add("Width",LorentzW, 0.01);
	(*Lorentzian)[2].setUpperLimit(8);
	Lorentzian->Add("Alpha",LorentzA);*/
}


virtual void 
tearDown()
{
 /*   delete Gaussian;
    delete Lorentzian;*/
 std::cerr << "teardown running";
}

/*  void testConstructor();
  void testLinking();
  void  testExtract();
*/
public:
  MinuitParameterSet *Gaussian;
  double  GaussE;
  double  GaussY;
  double  GaussW;
  double  GaussA;
  MinuitParameterSet *Lorentzian;
  double  LorentzE;
  double  LorentzY;
  double  LorentzW;
  double  LorentzA;
};

#endif	//MN_LinkTest_H_
