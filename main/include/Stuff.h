/*! @brief Function prototypes for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef ESsistMe_STUFF_H
#define ESsistMe_STUFF_H
//#include <glog/logging.h>
#include <string>
using namespace std;

/**
 *  @brief   Check the command line arguments and exit if in error
 *
 * @param[in]  argc    as in main()
 * @param[in]  argv    as in main()
 * @return the created processor or NULL if fails
 */

void
CheckArgumentList(int argc, char** argv);

void
OpenSystemFiles(const char* FileName, const char* LogFileName, string Heading);

void CloseSystemFiles(string Trailing);

/**
 *  @brief  Assemble the root part of the filename used for different goals
 *
 *  @param[in]  DataFileName   Name of the simulator command file
*/
  string
GetFileNameRoot(string DataFileName="");

/**
 *  @brief  Return application name and version
 */
  std::string
GetAppName(void);

/*
 *  @brief  Return application name and version
 * /
  std::string
GetAppVersion(void);
*/

bool FinishProgram(void);

#endif //  ESsistMe_STUFF_H
