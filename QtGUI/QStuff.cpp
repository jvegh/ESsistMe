/*!  @brief Some utility routines for the ESsistMe tool
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include "QStuff.h"
//#include <QStandardPaths>
bool UNIT_TESTING;
using namespace std;


void
SetupSystemDirectories(void)
{
    QDir dir;
    string UserHome = dir.homePath().toStdString();
    string Temporary = dir.tempPath().toStdString();
    string Starting = dir.currentPath().toStdString();
    string Executable = QCoreApplication::applicationDirPath().toStdString();
cerr << UserHome << std::endl;
cerr << Temporary << std::endl;
cerr << Starting << std::endl;
cerr << Executable << std::endl;
}
