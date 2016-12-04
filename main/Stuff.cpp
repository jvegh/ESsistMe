/*!  @brief Some utility routines for the ESsistMe tool
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <fstream>      // std::ifstream
#include <gtest/gtest.h>
#include <glog/logging.h>
#include <stdlib.h>
#include "Config.h"
#include "Stuff.h"

bool UNIT_TESTING;
using namespace std;

  void
CheckArgumentList(int argc, char** argv)
{
  ostringstream  logintext;
  if(argc < 3)
  {
    logintext  << " correct usage:\n" << argv[0] << " FileName.Eyo  NProc";
    cout << logintext.str() << endl; // Print out a kind of help, no logging yet
    exit (EXIT_FAILURE);
  }
}

void
OpenSystemFiles(const char* FileName, const char* LogFileName, string Heading)
{
  // Prepare and start logging
//  string FileRoot = GetFileNameRoot(FileName); // Prepare log file name
  FLAGS_alsologtostderr = 1; // Log also to the screen
  testing::internal::CaptureStdout();
  google::InstallFailureSignalHandler();
  google::SetLogDestination(0, (string(LogFileName)).c_str());
  google::InitGoogleLogging(FileName);
  LOG(INFO) << " " << Heading;
  LOG(INFO) << " Log file prefix for this run: \"" << LogFileName << "\"";
  LOG(INFO) << " <LOG message format: glog header] ><"  << GetAppName() << " message>";
}

//http://stackoverflow.com/questions/3803465/how-to-capture-stdout-stderr-with-googletest

void CloseSystemFiles(string Trailing)
{
  std::string output = testing::internal::GetCapturedStdout(); // Write out the captured log info
  LOG(INFO) <<  endl << output;
  LOG(INFO) << " " << Trailing;
}

  string
GetAppName(void)
{
  std::ostringstream appname;
  appname << PROJECT_NAME << " V" << PROJECT_VERSION;
  return appname.str();
}

/*  string
GetAppVersion(void)
{
  std::ostringstream appversion;
  appversion << " V" << PROJECT_VERSION;
  return appversion.str();
}*/



// Prepare a kind of file name fragment for created files
string GetFileNameRoot(string DataFileName)
{
  size_t fbegin = DataFileName.find_last_of('/');
  string FileNameRoot; 
  if(0==fbegin)
    FileNameRoot = DataFileName;
  else
    FileNameRoot = DataFileName.substr(fbegin+1); // Remove path first, if any
  fbegin = FileNameRoot.find_last_of('.');
  if(fbegin)
    FileNameRoot = FileNameRoot.substr(0,fbegin); // Remove extension, if any
    ostringstream FileNameFragment;
    if(UNIT_TESTING)
      FileNameFragment << GetAppName() << "_" << FileNameRoot << "_tst" ;
    else
      FileNameFragment << GetAppName() << "_GUI_" << FileNameRoot;

    string S = FileNameFragment.str();
    size_t found = S.find(" "); // Replace " " with "-" in file name
    S.replace(found,1,"-"); 
    return S;
}

