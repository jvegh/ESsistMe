
#include <iostream>
#include <sstream>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include "Stuff.h"
//#include "../main/include/Config.h"
extern bool UNIT_TESTING;	// Whether in course of unit testing
//extern bool GUI_MODE;		// Whether running under Qt5 GUI

using namespace std;
/**
 * @brief This is the main file of the ESsistMe tool Minuit extensions
 * 
 * @param argc Number of parameters
 * @param argv parameters, #1 is the name of the command file
 * @return int The result of the execution
 */
  char arg0[20]; // Application name
  char arg1[40]; // Test file name
  char arg2[10]; // No of cores

  // gtest runs in multithread mode
int main(int argc, char **argv) {
//  GUI_MODE = false;    // During testing, no GUI used
  UNIT_TESTING = true; // During unit testing, disable output to the screen
//  CheckArgumentList(argc, argv);

  ostringstream Heading;
  Heading << GetAppName() << " Minuit tests has started";
  string LogFileName = "LOG_"+GetFileNameRoot();
  string FileRoot = GetFileNameRoot() +'_'; // Prepare log file name
  OpenSystemFiles(FileRoot.c_str(), LogFileName.c_str(), Heading.str());
  testing::InitGoogleTest(&argc, argv);
  // The simulator functions also prepare some LaTeX plots, open the plotter
  // if(UNIT_TESTING) all interfaces are valid, but there is no functionality, no file
  int returnValue;
    //Do whatever setup here you will need for your tests here
    //
    //
  returnValue =  RUN_ALL_TESTS();
    //Do Your teardown here if required
    //
    //
  ostringstream Trailing;
  Trailing << GetAppName() << " Minuit tests has terminated";
  CloseSystemFiles(Trailing.str());
  return returnValue;
}

