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
//  testing::InitGoogleTest(&argc, argv);
  // Open logging. It seems to be OK, open logging
  ostringstream Heading;
  Heading << GetAppName() << " GUI has started\n";
  string LogFileName = string("Log_")+string("TestFile");
  SetupSystemDirectories();
  ESpectrumBase MySp;
  cerr << MySp.X_Get(5) << endl;
  cerr << MySp.X_Get_Kinetic(5) << endl;
  cerr << MySp.X_Get_Binding(5) << endl;
//  OpenSystemFiles(GetFileNameRoot(appname.str()).c_str(), LogFileName.c_str(), Heading.str());
  app.setApplicationName(appname.str().c_str());
  appname << "[*]";
  ESsistMe_MainWindow mainWin;
  mainWin.setWindowTitle(appname.str().c_str());
  mainWin.show();
  return app.exec();
}

