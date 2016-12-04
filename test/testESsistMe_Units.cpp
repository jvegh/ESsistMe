
#include <iostream>
#include <sstream>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include "../main/include/Stuff.h"
#include "../main/include/Config.h"
//#include "Constants.h"
extern bool UNIT_TESTING;	// Whether in course of unit testing

using namespace std;
/**
 * @brief This is the main file of the many-core simulator
 * 
 * @param argc Number of parameters
 * @param argv parameters, #1 is the name of the command file
 * @return int The result of the execution
 */

  // gtest runs in multithread mode, so we must have per thread (i.e. per test case) Processor
int main(int argc, char **argv) {
  UNIT_TESTING = true; // During unit testing, disable output to the screen

  ostringstream Heading;
  Heading << GetAppName() << " tool tests has started";
  string LogFileName = "LOG_"+GetFileNameRoot();
  string FileRoot = GetFileNameRoot() +'_'; // Prepare log file name
  OpenSystemFiles(FileRoot.c_str(), LogFileName.c_str(), Heading.str());
  testing::InitGoogleTest(&argc, argv);
  // if(UNIT_TESTING) all interfaces are valid, but there is no functionality, no file
    //Do whatever setup here you will need for your tests here
    //
    //
  int returnValue =  RUN_ALL_TESTS();
    //Do Your teardown here if required
    //
    //
  ostringstream Trailing;
  Trailing << GetAppName() << " tool tests has terminated";
  CloseSystemFiles(Trailing.str());
  return returnValue;
}

