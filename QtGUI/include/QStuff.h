/*! @brief Function prototypes for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef ESsistMe_QSTUFF_H
#define ESsistMe_QSTUFF_H
#include <QDir>
#include <QCoreApplication>
#include <fstream>      // std::ifstream
#include <stdlib.h>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::ostringstream
#include "Config.h"


using namespace std;
/*!
 * \brief The SystemDirectories contains some system-wide subdirectories
 */
struct SystemDirectories
{
    string Home;    // The nominal home directory of user
    string Work;    // Where the work files go
    string Temp;    // For temporary files
    string Install; // Where the system is installed
    string SystemData;    // for important data
    string SystemDocs;    // for documentation
    string UserData; // User-provided data
    string UserOutput; // User-provided data
    string LogFile; // The log messages
    bool SystemDataFound;   // If system data are available
    bool SystemDocsFound;   // If system docs are available
};


    void
SetupSystemDirectories(QWidget* parent);
/*    void
SetupSettings(void);*/
#endif //  ESsistMe_QSTUFF_H
