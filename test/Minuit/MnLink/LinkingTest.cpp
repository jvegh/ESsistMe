#include "GaussFcn.h"
#include "GaussDataGen.h"
#include "FCNMonitorBase.h"
#include "FunctionMinimum.h"
#include "MnUserParameterState.h"
#include "MnPrint.h"
#include "MnMigrad.h"
#include "MnMinos.h"
#include "MnContours.h"
#include "MnPlot.h"
#include "MinosError.h"
#include "ContoursError.h"

//#include <cassert>
#include	<stdio.h>
#include	<iostream>
void	DumpParameterSet(MinuitParameterSet &S, std::string prefix = "");
void	DumpParameter(MinuitParameter &P, std::string prefix = "");
void TestLinkingStatic();
void TestLinkingDynamic();
#include <gtest/gtest.h>
#include <glog/logging.h>
extern std::string pmLinkStrings[];

// Making Core tests for the simulator
class MinuitLinkTest  : public testing::Test
{
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
public:

    virtual void SetUp()
   {
//     LOG(INFO) << "Setup";
  GaussE=100.0;
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
	Lorentzian->Add("Alpha",LorentzA);
   }

   virtual void TearDown()
   {
    delete Gaussian;
    delete Lorentzian;
//     LOG(INFO) << "TearDown";
   }
};

#if MINUIT_EXTENDED
TEST_F(MinuitLinkTest, Initialize)
{
  double MyValue = 100;
  char MyName[] ="MyName2";
  int MyIDnum = 12;
  MinuitParameter MyPar1(MyIDnum, MyName, MyValue);
  EXPECT_EQ(MyValue, MyPar1.value());
  EXPECT_EQ(MyIDnum, MyPar1.number());
  EXPECT_EQ(*MyName, *MyPar1.name()); 
  EXPECT_TRUE(MyPar1.isConst());
  EXPECT_FALSE(MyPar1.isFixed());
  EXPECT_FALSE(MyPar1.hasLimits());
  EXPECT_FALSE(MyPar1.hasLowerLimit());
  EXPECT_FALSE(MyPar1.hasUpperLimit());
  EXPECT_EQ(pmNoLinkChar, MyPar1.GetLinkModeChar());
//  EXPECT_STREQ(pmLinkStrings[0].c_str(),MyPar1.GetLinkModeString().c_str());
  EXPECT_STREQ(std::string("100.00").c_str(),MyPar1.GetFormattedItem(MinuitParameter::pmValue ).c_str());
  EXPECT_STREQ(std::string("100.00").c_str(),MyPar1.GetFormattedItem(MinuitParameter::pmValue, true ).c_str());
  EXPECT_STREQ(std::string("100.00").c_str(),MyPar1.GetFormattedItem(MinuitParameter::pmError ).c_str());
}
#endif

TEST_F(MinuitLinkTest, Static)
{
  EXPECT_EQ( GaussE, (*Gaussian)[0].GetParameterValue() );
  EXPECT_EQ( GaussY, (*Gaussian)[1].GetParameterValue() );
  EXPECT_EQ(GaussW, (*Gaussian)[2].GetParameterValue() );
  EXPECT_EQ( GaussA, (*Gaussian)[3].GetParameterValue() );
  EXPECT_EQ( true, (*Gaussian)[1].isFixed() );
	(*Gaussian)[1].release();
   EXPECT_EQ( false, (*Gaussian)[1].isFixed() );
//   TestLinkingStatic();
}

TEST_F(MinuitLinkTest, Dynamic)
{
   Lorentzian->LinkParameter(0, '+', Gaussian, 0); // Link params in "Add" mode
  Lorentzian->LinkParameter(2, '*', Gaussian, 0); // Link params in "Multiply" mode
  EXPECT_EQ( LorentzE, (*Lorentzian)[0].GetParameterValue() );
  EXPECT_EQ( LorentzE-GaussE, (*Lorentzian)[0].GetParameterValue(true) );
  EXPECT_TRUE( (*Lorentzian)[0].isFixed() );
	(*Lorentzian)[0].release();
  EXPECT_FALSE( (*Lorentzian)[0].isFixed() );
  EXPECT_EQ( '+', (*Lorentzian)[0].GetLinkModeChar() );
  EXPECT_EQ( MinuitParameter::pmAddLink, (*Lorentzian)[0].GetLinkMode() );
  EXPECT_TRUE((*Gaussian)[0].IsMaster() );
  EXPECT_TRUE( (*Lorentzian)[0].IsSlave() );
  // Now unlink and check the same
    Lorentzian->UnlinkParameter(0);   
    Lorentzian->UnlinkParameter(2);   
   EXPECT_EQ( MinuitParameter::pmNoLink, (*Lorentzian)[0].GetLinkMode() );
   EXPECT_FALSE( (*Gaussian)[0].IsMaster() );
   EXPECT_FALSE( (*Lorentzian)[0].IsSlave() );
   EXPECT_EQ( LorentzE, (*Lorentzian)[0].GetParameterValue() );
}


TEST_F(MinuitLinkTest, Extract)
{
 	// generate the data (100 data points)
  GaussDataGen gdg(100);

  std::vector<double> pos = gdg.positions();
  std::vector<double> meas = gdg.measurements();
  std::vector<double> var = gdg.variances();
   
  // create FCN function  
  GaussFcn theFCN(meas, pos, var);
  // create initial starting values for parameters
  double x = 0.;
  double x2 = 0.;
  double norm = 0.;
  double dx = pos[1]-pos[0];
  double area = 0.;
  for(unsigned int i = 0; i < meas.size(); i++) {
    norm += meas[i];
    x += (meas[i]*pos[i]);
    x2 += (meas[i]*pos[i]*pos[i]);
    area += dx*meas[i];
  }
  double mean = x/norm;
  double rms2 = x2/norm - mean*mean;
  double rms = rms2 > 0. ? sqrt(rms2) : 1.;

 	    MinuitParameterSet Gaussian((char*)std::string("Gaussian").c_str());
	    Gaussian.Add("Energy",mean, 0.1);
        Gaussian[0].setLimits(-60,10);
	    Gaussian.Add("Width",rms, 0.01);
        Gaussian[1].setLowerLimit(0.01);
	    Gaussian.Add("Yield",area, 0.1);
        Gaussian[1].setUpperLimit(150);
        MnUserParameters upar;
        upar.add(Gaussian);
        // create MIGRAD minimizer
       // MnMigrad migrad(theFCN, upar);

       // minimize
       // FunctionMinimum min = migrad();

        // MnUserParameters upar1 = min.userParameters();

        // Now clear the pm fields
        // Gaussian[0].setLimits(-1,1); 
        Gaussian[0].setValue(1);
        upar.extract(Gaussian);
        printf("value = %f\n", Gaussian[0].value());
         printf("error = %f\n", Gaussian[0].error());
  EXPECT_EQ( Gaussian[0].value(), mean );

  EXPECT_EQ( Gaussian[0].lowerLimit(), -60. );
  EXPECT_EQ( Gaussian[0].upperLimit(), 10. );
}


/*  std::vector<double> pars = Gaussian.GetParameterValues();
  for(std::vector<double>::const_iterator ipar = pars.begin();
      ipar != pars.end(); ipar++)
      printf("%8.3f\n", (*ipar));
  */

void TestLinkingStatic()
{
	printf("Test linking\n");
	MinuitParameterSet Gaussian((char*)std::string("Gaussian").c_str());
	Gaussian.Add("Energy",100.0, 0.1);
	Gaussian[0].setLimits(50,200);
	Gaussian.Add("Yield",1000.0, 1.1);
	Gaussian[1].fix();
	Gaussian.Add("Width",5.0, 0.01);
	Gaussian[2].setUpperLimit(8);
	Gaussian.Add("Alpha",0.1);
	Gaussian[1].release();
	DumpParameterSet(Gaussian);

	MinuitParameterSet Lorentzian((char*)std::string("Lorentzian").c_str());
	Lorentzian.Add("Energy",104.0, 0.1);
	Lorentzian[0].setLimits(50,200);
	Lorentzian.Add("Yield",1000.0, 1.1);
	Lorentzian.Add("Width",5.0, 0.01);
	Lorentzian[2].setUpperLimit(8);
	Lorentzian.Add("Alpha",0.1);
	DumpParameterSet(Lorentzian);

    Lorentzian.LinkParameter(0, '+', &Gaussian, 0);
	DumpParameterSet(Lorentzian);
	DumpParameterSet(Gaussian);
    Lorentzian.UnlinkParameter(0);
	DumpParameterSet(Lorentzian);
	DumpParameterSet(Gaussian);
	
}

void TestLinkingDynamic()
{
	// generate the data (100 data points)
/*  GaussDataGen gdg(100);

  std::vector<double> pos = gdg.positions();
  std::vector<double> meas = gdg.measurements();
  std::vector<double> var = gdg.variances();
   
  // create FCN function  
  GaussFcn theFCN(meas, pos, var);
  // create initial starting values for parameters
  double x = 0.;
  double x2 = 0.;
  double norm = 0.;
  double dx = pos[1]-pos[0];
  double area = 0.;
  for(unsigned int i = 0; i < meas.size(); i++) {
    norm += meas[i];
    x += (meas[i]*pos[i]);
    x2 += (meas[i]*pos[i]*pos[i]);
    area += dx*meas[i];
  }
  double mean = x/norm;
  double rms2 = x2/norm - mean*mean;
  double rms = rms2 > 0. ? sqrt(rms2) : 1.;

    {
        // demonstrate standard minimization using MIGRAD
        // create Minuit parameters with names
	    MinuitParameterSet Gaussian((char*)std::string("Gaussian").c_str());
	    Gaussian.Add("Energy",mean, 0.1);
        Gaussian[0].setLimits(-60,10);
	    Gaussian.Add("Width",rms, 0.01);
        Gaussian[1].setLowerLimit(0.01);
	    Gaussian.Add("Yield",area, 0.1);
        Gaussian[1].setUpperLimit(150);
//        Gaussian.LinkParameter(1,'*', &Gaussian, 0);
//        Gaussian.UnlinkParameter(1);
//    	DumpParameterSet(Gaussian);

        MnUserParameters upar;
    	DumpParameterSet(Gaussian);
         upar.add(Gaussian);
    	DumpParameterSet(Gaussian);

       // create MIGRAD minimizer
        MnMigrad migrad(theFCN, upar);

       // minimize
        FunctionMinimum min = migrad();

        MnUserParameters upar1 = min.userParameters();
 //       printf("after  %f,%f,%f\n", upar1.parameters()[0].value(), upar1.parameters()[1].value(), upar1.parameters()[2].value());
        upar1.extract(Gaussian);
    	DumpParameterSet(Gaussian);

        // output
        std::cout<<"minimum: "<<min<<std::endl;
    }
    */
}
void	DumpParameter(MinuitParameter &P, std::string prefix)
{
    printf("%s: Label '%s'\n",prefix.c_str(),P.GetLabel());
	if(P.isConst())
	{
		printf("%s: Pm: value = %s [constant]\n",prefix.c_str(),P.GetFormattedValue().c_str());
	}
	else if(P.isFixed())
	{
		printf("%s: Pm: value = %s+/-%s [fixed]\n",prefix.c_str(),
			P.GetFormattedValue().c_str(),P.GetFormattedItem(MinuitParameter::pmError).c_str());
	}
	else if(P.hasLimits())
	{
		if(P.hasLowerLimit() && P.hasUpperLimit())
		{
			printf("%s: Pm: value = %s+/-%s [%s<x<%s]\n",prefix.c_str(),
			P.GetFormattedValue().c_str(),P.GetFormattedItem(MinuitParameter::pmError).c_str(),
			P.GetFormattedItem(MinuitParameter::pmLimLow).c_str(),P.GetFormattedItem(MinuitParameter::pmLimHigh).c_str());
		}
		else if(P.hasLowerLimit())
		{
			printf("%s: Pm: value = %s+/-%s [%s<x]\n",prefix.c_str(),
			P.GetFormattedValue().c_str(),P.GetFormattedItem(MinuitParameter::pmError).c_str(),
			P.GetFormattedItem(MinuitParameter::pmLimLow).c_str());
		}
		else if(P.hasUpperLimit())
		{
			printf("%s: Pm: value = %s+/-%s [x<%s]\n",prefix.c_str(),
			P.GetFormattedValue().c_str(),P.GetFormattedItem(MinuitParameter::pmError).c_str(),
			P.GetFormattedItem(MinuitParameter::pmLimHigh).c_str());
		}
	}
	else
	{
		printf("%s: Pm: value = %s+/-%s [free]\n",prefix.c_str(),
        P.GetFormattedValue().c_str(),P.GetFormattedItem(MinuitParameter::pmError).c_str());
	}
	if(P.IsSlave())
	{
        MinuitParameter PSlave = P;
        PSlave.SetMasterPointer(NULL); // Simulate non-linked 
        MinuitParameter *PMaster = &P.GetMasterPointer()->Parameters[P.GetMasterParameterNo()];
        if(MinuitParameter::pm2ParLinkMask & P.GetLinkMode())
        {
            MinuitParameter *PMaster1 = &P.GetMasterPointer()->Parameters[P.GetMasterParameterNo()+1];
            printf("%s: Pm: '%c' Linked to pm#%d+1 of '%s' :\n",prefix.c_str(),
 		    P.GetLinkModeChar(),
		    P.GetMasterParameterNo(), P.GetMasterPointer()->GetLabel());
            DumpParameter(*PMaster,   /*prefix + */std::string("    Master 0:"));
            DumpParameter(*PMaster1,  /*prefix+*/std::string("    Master 1:"));
        }
        else
        {
            printf("%s: Pm: '%c' Linked to pm#%d of '%s' :\n",prefix.c_str(),
 		    P.GetLinkModeChar(),
		    P.GetMasterParameterNo(), P.GetMasterPointer()->GetLabel());
            DumpParameter(*PMaster,   /*prefix+*/std::string("    Master 0:"));
        }
        DumpParameter(PSlave,  /*prefix+*/std::string("    Unlinked pm:"));
	}
	/*else
		printf("%s: Pm: Not linked\n",prefix.c_str());*/
	if(P.GetMasterCount())
		printf("%s: Pm: Masters %d parameter sets\n",prefix.c_str(),P.GetMasterCount());
	printf("%s: Pm: -------------\n",prefix.c_str());
}

void	DumpParameterSet(MinuitParameterSet &S, std::string prefix)
{
	std::string a = S.GetLabel();
    printf("%s: Dumping set '%s' having %d parameters\n", prefix.c_str(), a.c_str(), S.GetCount());
	for(unsigned int i = 0; i<S.GetCount(); i++)
	{
		if(S.GetCount() > 1)
            printf("%s:<<<<<<<<<<<<<<<< %s/Parameter #%d\n", prefix.c_str(), a.c_str(), i);
		DumpParameter(*S.GetParameter(i), prefix);
	}
/*	if(S.GetAffectedList()->GetCount())
	{
		wxLogMessage("Set: Affects %d parameter sets",S.GetAffectedList()->GetCount());
	}
	if(S.GetAttachmentList()->GetCount())
	{
		wxLogMessage("Set: Attached  %d parameter sets",S.GetAttachmentList()->GetCount());
	}
*/
	printf("Set :::::::::::::\n");
}

