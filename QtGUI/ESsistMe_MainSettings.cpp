#include "ESsistMe_mainwindow.h"
#include "ui_ESsistMe_mainform.h"
#include "qcustomplot.h" // the header file of QCustomPlot.
#include "QStuff.h"
#include "Stuff.h"


void ESsistMe_MainWindow::readSettings()
{
    QSettings settings(QSettings::IniFormat , QSettings::UserScope, PACKAGE_NAME, PROJECT_NAME "V" PROJECT_VERSION);
    if (settings.allKeys().isEmpty())
        return; // A virgin startup
    qInfo() << "Reading 'MainWindow' settings";
    // First read the application-wide settings
    readSettings(&settings);
     QString ProjectSettings(string(Directories.Work + string("/data/") + string(PROJECT_NAME "V" PROJECT_VERSION ".ini")).c_str());
     QFileInfo check_file(ProjectSettings);
     if (check_file.exists() && check_file.isFile())
     {
         qInfo() << "Reading project's 'MainWindow' settings";
         QSettings settings(ProjectSettings, QSettings::IniFormat );
         readSettings(&settings);
     }
}

// This makes the real reading, either application- or project-wide settings
void ESsistMe_MainWindow::readSettings(QSettings *settings)
{
    settings->beginGroup("MainWindow");
restoreState(settings->value("geometry").toByteArray());
    resize(settings->value("size", QSize(1024, 768)).toSize());
    move(settings->value("pos", QPoint(0, 0)).toPoint());
/*	working_directory=settings.value("dir").toString();
if (working_directory.isEmpty())
    working_directory=QDir::homePath();
ppSpinBox->setValue(settings.value("NoiseFactor",0).toInt());
levelsSpinBox->setValue(settings.value("levels",6).toInt());
plot3D->nlevels=levelsSpinBox->value();
posnegComboBox->setCurrentIndex(settings.value("posneg",0).toInt());
plot3D->pos_neg=posnegComboBox->currentIndex();
floorSpinBox->setValue(settings.value("floor",5.0).toDouble());
plot3D->floor=floorSpinBox->value();
ceilingSpinBox->setValue(settings.value("ceiling",100.0).toDouble());
plot3D->ceiling=ceilingSpinBox->value();
multiplierSpinBox->setValue(settings.value("multiplier",2.0).toDouble());
plot3D->multiplier=multiplierSpinBox->value();
sliceComboBox->setCurrentIndex(settings.value("slice",0).toInt());
QString colourSchemeName=settings.value("colourScheme","white").toString();
ColourScheme* scPt=NULL;
if (!colourSchemeList.size())
    qDebug()<<"colourSchemeList is empty!";
for (int i=0; i<colourSchemeList.size(); i++) {
    if (colourSchemeName==colourSchemeList.at(i)->name)
        scPt=colourSchemeList.at(i);
}
if (!scPt)
    scPt=colourSchemeList.at(0); //set white scheme by default
plot2D->setScheme(*scPt);
plot3D->setScheme(*scPt);
*/
    settings->endGroup();
}


void ESsistMe_MainWindow::writeSettings()
{
    QSettings settings(QSettings::IniFormat , QSettings::UserScope, PACKAGE_NAME, PROJECT_NAME "V" PROJECT_VERSION);
    qInfo() << "Writing 'MainWindow' settings";
        settings.beginGroup("MainWindow");
        settings.setValue("size", size());
        settings.setValue("pos", pos());
/*	settings.setValue("dir", working_directory);
    settings.setValue("geometry", saveState());
    settings.setValue("NoiseFactor", ppSpinBox->value());
    settings.setValue("colourScheme", activePlot()->colourScheme.name);
    */
        settings.endGroup();
}


void global_options_::readSettings()
{
    QSettings settings(QSettings::IniFormat , QSettings::UserScope, PACKAGE_NAME, PROJECT_NAME PROJECT_VERSION);
        settings.beginGroup("Options");
    ticks=settings.value("ticks",5).toInt();
    hasXaxis=settings.value("xaxis",true).toBool();
    hasYaxis=settings.value("yaxis",true).toBool();
    hasExtraTicks=settings.value("hasExtraTicks",true).toBool();
    extraTicks=settings.value("extraTicks",5).toInt();
    xunits=settings.value("xUnits",UNITS_EV).toInt();
    yunits=settings.value("yUnits",UNITS_COUNTS).toInt();
    axisFont=settings.value("axisFont",QString("")).toString();
    antialiasing=settings.value("Antialiasing",false).toBool();
    customPrintLogo=settings.value("CustomPrintLogo","").toString();
    printPars=settings.value("PrintPars",true).toBool();
    printLineWidth=settings.value("PrintLineWidth",1).toInt();
    smoothTransformations=settings.value("SmoothTransformations",true).toBool();
    animatedMarkers=settings.value("AnimatedMarkers",false).toBool();
    style=settings.value("Style","").toString();
    useBold1D=settings.value("UseBold1D",true).toBool();
    useSkyline=settings.value("UseSkyline",false).toBool();
    mixFidSpec=settings.value("MixFIDandSpectra",false).toBool();
    useSystemPrintPars=settings.value("UseSystemPrintPars",true).toBool();
        print1Dmode=settings.value("Print1DMode",0).toInt();
    yLeftLabels=settings.value("YLeftLabels",false).toBool();
    QStringList defaultToolBar;
    defaultToolBar<<"DC Offset"<<"Resize (dir)"<<"LB (dir)"<<"FT/FFT (dir)"<<"Phase (dir)";
//	procToolBarList=settings.value("ProcessingToolBar",defaultToolBar).toStringList();
        settings.endGroup();
}

void global_options_::writeSettings()
{
    QSettings settings(QSettings::IniFormat , QSettings::UserScope, PACKAGE_NAME, PROJECT_NAME);
        settings.beginGroup("Options");
        settings.setValue("ticks", int(ticks));
    settings.setValue("xaxis", hasXaxis);
    settings.setValue("yaxis", hasYaxis);
    settings.setValue("hasExtraTicks", hasExtraTicks);
    settings.setValue("extraTicks", int(extraTicks));
    settings.setValue("xUnits",xunits);
    settings.setValue("yUnits",yunits);
    settings.setValue("axisFont",axisFont);
    settings.setValue("CustomPrintLogo",customPrintLogo);
    settings.setValue("PrintPars",printPars);
    settings.setValue("PrintLineWidth",printLineWidth);
    settings.setValue("UseBold1D",useBold1D);
    settings.setValue("UseSystemPrintPars",useSystemPrintPars);
        settings.setValue("Print1DMode",print1Dmode);
    settings.setValue("YLeftLabels",yLeftLabels);
        settings.endGroup();
}

