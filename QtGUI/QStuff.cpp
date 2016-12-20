/*!  @brief Some utility routines for the ESsistMe tool
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <Stuff.h>
#include "QStuff.h"

bool UNIT_TESTING;
using namespace std;
#include <QApplication>
#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QMessageBox>
#include <QTime>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <sstream>
QDir dir;

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
} Directories;

// This is the logging facility provided by Qt.
//
// http://www.qtcentre.org/threads/19534-redirect-qDebug()-to-file
//
// qDebug is disabled in release mode
// http://beyondrelational.com/modules/2/blogs/79/Posts/19245/how-to-put-logging-in-c-application-using-qt50.aspx

void MessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile outfile(Directories.LogFile.c_str());
    if(outfile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
    QTextStream ts(&outfile);
    switch (type) {
    case QtDebugMsg:
        ts << "Debug: ";
        break;
    case QtInfoMsg:
        break;
    case QtWarningMsg:
        ts << "Warning: ";
    break;
    case QtCriticalMsg:
        ts << "Critical: ";
    break;
    case QtFatalMsg:
        ts << "Fatal: ";
    }
    ts <<  QTime::currentTime().toString("hh:mm:ss ");
    ts << "> " << msg << '\n';
    cerr << msg.toStdString().c_str() << '\n';
      ts.flush();
    }
    else
    {
        // Could not open file
    }
    if(type==QtFatalMsg)
        abort();
}


/*!
 * \brief SetupSystemDirectories
 * \param parent the parent widget (usually QMainWindow)
 *
 * It verifies whether the system directory structure exists
 * and sets it up if not
 *
 * @verbatim
 *  |InstallDir
 *  |--/bin
 *  |../data
 *  |../docs
 * @endverbatim
 *
 * If this directory does not exist, it is usually an installation error
 *
 * It is assumed to use a project-like directory structure. I.e., the user has a
 * per session subdirectory for the evaluation sessions. It comprises 'data' subdirectory
 * for persistent data (furnished by the user) and 'output' subdirectory for the generated files
 * @verbatim
 *  |WorkDir
 *  |../data
 *  |../output
 * @endverbatim
 *
 * The work directory is normally the directory where the tool is invoked from.
 * If that directory is either the 'InstallDir/bin' or 'UserHome', (in order to avoid
 * polluting those directories) the tool creates a new subdirectory 'UserHome/PackageName'
 *
 * The tool needs also a temporary directory. It will be created in
 *  @verbatim
 *  |TempDir
 *  |--/PackageName
 *  @endverbatim
 * if the temporary directory is available for the user; otherwise in
 *  @verbatim
 *  |UserHome
 *  |--/@PACKAGE_NAME@
 *  |----/temp
 *  @endverbatim
 *
 * In addition, the application maintains a 'last state' information in
 * @verbatim
 *  |UserHome
 *  |--/.config
 *  |----/@PACKAGE_NAME@
 *  |------/@PROJECT_NAME@@PROJECT_VERSION@.ini
 *  @endverbatim

 */
void
SetupSystemDirectories(QWidget* parent)
{
    bool WorkDirRelocated = false;
    bool WorkDirCreated = false;
    Directories.Home = dir.homePath().toStdString();
    Directories.Install = QCoreApplication::applicationDirPath().toStdString();
    // Work directory found or not needed
    Directories.Work = dir.currentPath().toStdString(); // Presuppose current directory as work
    string OldWorkDir = Directories.Work;
    if(!Directories.Home.compare(Directories.Work) || !Directories.Install.compare(Directories.Work))
    {
        Directories.Work = Directories.Home + '/' + PACKAGE_NAME;
        WorkDirRelocated = true;
        if(!QDir(Directories.Work.c_str()).exists())
         { // No such directory, create it
            WorkDirCreated = true;
            if(!QDir(QString(Directories.Home.c_str())).mkdir(QString(Directories.Work.c_str())) )
            { // Could not create work directory
                       qFatal("Could not create work directory; exiting");
            }
        }
    }
    // Now the .log file goes to the right place
    string DirName = Directories.Work +'/' + GetAppName();
    size_t found = DirName.find(" "); // Replace " " with "-" in file name
    DirName.replace(found,1,"-");
    Directories.LogFile = DirName + QDate::currentDate().toString("_yyyy-MM-dd").toStdString() + QTime::currentTime().toString("_hh:mm.log").toStdString();

    // Before this call, Directories.Work must be set up!
    qInstallMessageHandler(MessageHandler);
    qInfo() << GetAppName().c_str() << " successfully started from '" << OldWorkDir.c_str() << "'";
    // Now the logging file and directory is set up, can start logging
    if(WorkDirCreated)
        qDebug()  << "Created new work directory " << Directories.Work.c_str();
    if(WorkDirRelocated)
        qDebug() << "Work directory relocated to " << Directories.Work.c_str();

    Directories.SystemData = Directories.Install + "/../" + "data";
    QMessageBox::StandardButton ret;
    Directories.SystemDataFound = true;
    // Handle system data directory
    if(!QDir(QString(Directories.SystemData.c_str())).exists())
    {
        qDebug()  << "Not found system data directory " << Directories.SystemData.c_str();
        Directories.SystemDataFound = false;
    ret = QMessageBox::warning(parent, "Application",
                     "System data directory not found\n"
                        "Shall I continue?",
//                                   int ret = QMessageBox::warning(parent, tr("Application"),
  //                                                  tr("System data directory not found\n"
     //                                                  "Shall I continue?"),
                     QMessageBox::Yes  | QMessageBox::Cancel);
        if (ret != QMessageBox::Yes)
     {
            qFatal("Missing system data directory; exiting");
     }
    }
    else
    {
        qDebug() << "Found system data directory " << Directories.SystemData.c_str() ;
    }

    // Handle system docs directory
    Directories.SystemDocs = Directories.Install + "/../" + "docs";
    Directories.SystemDocsFound = true;
    // Handle system docs directory
    if(!QDir(QString(Directories.SystemDocs.c_str())).exists())
        {
        qDebug() << "Not found system docs directory " << Directories.SystemDocs.c_str();
        Directories.SystemDocsFound = false;
        ret = QMessageBox::warning(parent, "Application",
                         "System docs directory not found\n"
                            "Shall I continue?",
    //                                   int ret = QMessageBox::warning(parent, tr("Application"),
      //                                                  tr("System docs directory not found\n"
         //                                                  "Shall I continue?"),
                         QMessageBox::Yes  | QMessageBox::Cancel);
            if (ret != QMessageBox::Yes)
        {
                qCritical() << "System docs directory '" << Directories.SystemDocs.c_str() << "' missing; exiting";
        }
        }
    else
    {
        qDebug()  << "Found system docs directory '" << Directories.SystemDocs.c_str();
    }

    // Handle temporary directory
    Directories.Temp = dir.tempPath().toStdString();
    bool SystemTempFound = false;
    string DirTempRoot = Directories.Temp;
    bool TempDirCreated = false;
    if(QDir(QString(Directories.Temp.c_str())).exists())
    {
        Directories.Temp = dir.tempPath().toStdString() + '/' + PACKAGE_NAME;
        SystemTempFound = true;
     }
    else
        Directories.Temp = Directories.Work + '/' + PACKAGE_NAME + "/temp";
    QDir temppath(Directories.Temp.c_str());
    if (!temppath.exists()){
        temppath.mkdir(Directories.Temp.c_str());
        TempDirCreated = true;
      }

    if(!SystemTempFound)
        qDebug() << "Not found system-wide 'temp'";
    if(TempDirCreated)
        qDebug() << "Created new temp dir " << Directories.Temp.c_str();
    else
        qDebug() << "Using old temp dir " << Directories.Temp.c_str();
    qDebug() << "The temporary root dir is  " << DirTempRoot.c_str();
    // Handle user data directory
    bool UDataDirFound = false;
    bool UDataDirCreated = false;
    Directories.UserData = Directories.Work + '/' + "data";
    QDir datapath(Directories.UserData.c_str());
    if (!datapath.exists()){
        if(datapath.mkdir(Directories.UserData.c_str()))
        {
        UDataDirCreated = true;
        }
        else
        {
            qFatal("User data directory cannot be created");
        }
      }
    else UDataDirFound = true;

    if(!UDataDirFound)
        qDebug() << "Not found user data directory " << Directories.UserData.c_str() ;
    if(UDataDirCreated)
        qDebug() << "Created new user data directory " << Directories.UserData.c_str();
    else
        qDebug() << "Using old user data directory " << Directories.UserData.c_str();

    // Handle user output directory
    bool UOutputDirFound = false;
    bool UOutputDirCreated = false;
    Directories.UserOutput = Directories.Work + '/' + "output";
    QDir outpath(Directories.UserOutput.c_str());
    if (!outpath.exists()){
        if(datapath.mkdir(Directories.UserOutput.c_str()))
        {
        UOutputDirCreated = true;
        }
        else
        {
            qFatal("User output directory cannot be created");
        }
      }
    else UOutputDirFound = true;

    if(!UOutputDirFound)
        qDebug() << "Not found user output directory " << Directories.UserOutput.c_str() ;
    if(UOutputDirCreated)
        qDebug() << "Created new user output directory" << Directories.UserOutput.c_str();
    else
        qDebug() << "Using old user output directory " << Directories.UserOutput.c_str();

    if(!Directories.SystemDataFound)
        qWarning() << "Data-dependent services are not available";
    if(!Directories.SystemDocsFound)
        qWarning() << "System documentation files are not available";
    qInfo() << "Setting of directories of the tool " ;
    qInfo() << "  Install:   " << Directories.Install.c_str();
    qInfo() << "  Work:      " << Directories.Work.c_str();
    qInfo() << "  Output:    " << Directories.UserOutput.c_str();
    qInfo() << "  Temporary: " << Directories.Temp.c_str();
    qInfo() << "  Config:    " << Directories.Home.c_str() <<"/.config/" << PACKAGE_NAME;

}

// After this setup, QSettings can be used from different places
// using the default constructor
/*void SetupSettings(void)
{
    QCoreApplication::setOrganizationName("vjSoft");
    QCoreApplication::setOrganizationDomain("https://github.com/jvegh/ESsistMe/");
    QCoreApplication::setApplicationName("ESsistMe");
}
*/
