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
  std::ostringstream appname;
  QApplication app(argc, argv);
  appname << GetAppName();
  ESsistMe_MainWindow mainWin;
//  testing::InitGoogleTest(&argc, argv);
  // Open logging. It seems to be OK, open logging
//  ostringstream Heading;
//  Heading << GetAppName() << " GUI has started\n";
//  string LogFileName = string("Log_")+string("TestFile");
//  OpenSystemFiles(GetFileNameRoot(appname.str()).c_str(), LogFileName.c_str(), Heading.str());
  app.setApplicationName(appname.str().c_str());
  appname << "[*]";
  mainWin.setWindowTitle(appname.str().c_str());
  SetupSystemDirectories(&mainWin);
  mainWin.show();
  return app.exec();
}

