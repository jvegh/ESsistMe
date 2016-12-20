//#include <gtest/gtest.h>
#include <QApplication>
#include "Config.h"
#include "Stuff.h"
#include "QStuff.h"
#include "ESpectrumBase.h"
#include <sstream>

#include "ESsistMe_mainwindow.h"

int main(int argc, char *argv[])
{

//  Q_INIT_RESOURCE(application);
  QApplication app(argc, argv);
  ESsistMe_MainWindow mainWin;
//  OpenSystemFiles(GetFileNameRoot(appname.str()).c_str(), LogFileName.c_str(), Heading.str());
   std::ostringstream appname;
  appname << GetAppName();
  app.setApplicationName(appname.str().c_str());
  appname << "[*]";
  mainWin.setWindowTitle(appname.str().c_str());
  SetupSystemDirectories(&mainWin); // Establish system and user directories
//  SetupSettings();  // Set up for using QSettings from different places
  mainWin.show();
  return app.exec();
}

