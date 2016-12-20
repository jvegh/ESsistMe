#include "ESsistMe_mainwindow.h"
#include "ui_ESsistMe_mainform.h"
#include <QMdiSubWindow>
#include <QtCore>
#include "qcustomplot.h" // the header file of QCustomPlot.
#include "QStuff.h"
#include "Stuff.h"

QVector<double> X0(250), Y0(250);
QVector<double> YConfUpper(250), YConfLower(250);
QVector<double> X1(50), Y1(50), Y1err(50), YResidual(50);

ESsistMe_MainWindow::ESsistMe_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ESsistMe_MainWindow)
{
    ui->setupUi(this);
    (void)statusBar();
    setWindowTitle( "ESsistMe" );
    setIconSize(QSize(24,24));
    setWindowIcon( QPixmap( ":/images/ESlogo.png" ) );
    SetFileMenu();
//    initFileFilterList();
    readSettings(); // Read window-related settings

}

ESsistMe_MainWindow::~ESsistMe_MainWindow()
{
    delete ui;
}

void ESsistMe_MainWindow::SetFileMenu(void)
{
fileNewWindowAction = new QAction( this);
fileNewWindowAction->setText( tr( "&New window" ) );
fileNewWindowAction->setIcon( QIcon( ":/images/window_new.png") );
fileNewWindowAction->setShortcut(QKeySequence::New);
fileNewWindowAction->setShortcuts(QKeySequence::New);
fileNewWindowAction->setStatusTip(tr("Create a new file"));
connect( fileNewWindowAction, SIGNAL( triggered() ), this, SLOT( fileNewWindow() ) );

fileOpenAction = new QAction( this);//, "fileOpenAction" );
fileOpenAction->setIcon( QIcon( ":/images/fileopen.png") );
fileOpenAction->setText( tr( "&Open..." ) );
fileOpenAction->setShortcut(QKeySequence::Open);
connect( fileOpenAction, SIGNAL( triggered() ), this, SLOT( fileOpenDialog() ) );

fileExitAction = new QAction( this);//, "fileExitAction" );
fileExitAction->setText(tr ("E&xit"));
fileExitAction->setShortcuts(QKeySequence::Quit);
connect( fileExitAction, SIGNAL( triggered() ), this, SLOT( fileExit() ) );

ui->fileMenu = ui->menubar->addMenu(tr("&File"));
ui->fileMenu->addAction(fileNewWindowAction);
ui->fileMenu->addAction(fileOpenAction);
ui->fileMenu->addSeparator();
ui->fileMenu->addAction(fileExitAction);

}



void ESsistMe_MainWindow::fileNewWindow()
{
 /*   static int wcounter=1;
    PlotArea* temparea=new PlotArea(this);
    QMdiSubWindow* subWindow=mdiArea->addSubWindow(temparea);
    mdiArea->setMinimumSize(QSize(160,120));
    mdiArea->setActiveSubWindow(subWindow);
    connect(temparea->plot2D, SIGNAL(openFileAsked(QString)), this, SLOT(fileOpen(QString)));
    connect(temparea->plot3D, SIGNAL(openFileAsked(QString)), this, SLOT(fileOpen(QString)));
    connect(temparea->plot2D, SIGNAL(onXaxisLabelClicked()), this, SLOT(cycChangeXAxis()));
    connect(temparea->plot3D, SIGNAL(onXaxisLabelClicked()), this, SLOT(cycChangeXAxis()));
    connect(temparea->plot3D, SIGNAL(onYaxisLabelClicked()), this, SLOT(cycChangeYAxis()));
//	update2d();
    subWindow->setWindowTitle(QString("Window%1").arg(wcounter++));
    subWindow->setWindowIcon(QIcon(":/images/gsim.png"));
    subWindow->raise();
    windowActivated(subWindow);
    subWindow->show(); // bug in Qt? see main.cpp for extra changes
    */
}

void ESsistMe_MainWindow::fileOpenDialog()
{
    QFileDialog file_dialog(
                this,
            "Choose a file",
             working_directory
                        );
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    QStringList filters;
    QString all_ext;
 /*   for (size_t i=0; i<fileFilterList.size(); i++) {
        if (fileFilterList.at(i)->canRead())
            all_ext+=fileFilterList.at(i)->extensions()+" ";
    }
    filters<<"Any known fileformats ("+all_ext+")";
    for (size_t i=0; i<fileFilterList.size(); i++) {
        if (fileFilterList.at(i)->canRead())
            filters.push_back(fileFilterList.at(i)->name()+" ("+fileFilterList.at(i)->extensions()+")");
    }

    file_dialog.setNameFilters(filters);

    QCheckBox newwindowbox("Open in new window");
    if (data->size())
        file_dialog.layout()->addWidget(&newwindowbox);

    QStringList list;
    if (file_dialog.exec())
        list = file_dialog.selectedFiles();
   if (list.isEmpty()) return;
    QStringList attData, procData;
    readAttachedProcessing(attData, procData);
    checkAttachedProcessing(attData, procData,list);
    if (newwindowbox.isChecked()) {
    fileNewWindow();
    }
    const size_t len=list.size();
    for (size_t i=0; i<len; i++){
        QString name=list.at(i);
            if (name.isEmpty())
                return;
        QString filter=file_dialog.selectedNameFilter();
    try{
        fileOpen(name, filter);//0-any other x: x-1 filter in fileFilterList
    }
    catch(MatrixException exc) {
        QMessageBox::critical(this,"Reading error", exc.what());
        return;
    }
    }
*/   statusBar()->showMessage("File has been loaded",2000);

}


void ESsistMe_MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeClose()) {
//        writeSettings();
        writeSettings();    // Qrite out window-related settings

        qInfo() << GetAppName().c_str() << " normally terminated by user";
        event->accept();

/*   CloseSystemFiles(GetAppName() + " simulator GUI version has terminated");
*/    } else {
        event->ignore();
    }
}


bool ESsistMe_MainWindow::maybeClose()
{
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("Closing this window will exit the tool.\n"
                        "Are you sure you want exit?"),
                     QMessageBox::Yes  | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes)
    {
      return true;
    }
        else if (ret == QMessageBox::Cancel)
            return false;
    return false;

}
void ESsistMe_MainWindow::fileOpen(QString name,  QString filter, int flags)
{
   /* BaseFileFilter* fpointer=NULL;
    if (filter.contains("Any known fileformats")) {
        for (size_t i=0; i<fileFilterList.size(); i++)
            if (fileFilterList.at(i)->matching(name.toLatin1()))
                fpointer=fileFilterList.at(i);
    }
    else {
        for (size_t i=0; i<fileFilterList.size(); i++) {
            if (filter.contains(fileFilterList.at(i)->name()))
                fpointer=fileFilterList.at(i);
        }
    }

    if (fpointer==NULL) {
        QMessageBox::critical(this,"Error", "Appropriate file format is not found");
        return;
        }
    if (!QFile::exists(name)) {
        QMessageBox::critical(this,"Failed",QString("File %1 doesn't exist").arg(name));
        return;
    }
    data->anyFileOpen(name,fpointer,flags);
    if (!data->size())
        return;

    if (flags&OpenUpdateWorkingDirectory) {
        QDir dir;
        working_directory=dir.absoluteFilePath(name);
    }
    if (flags&OpenSetActive) {
        const size_t k=data->size()-1;
        data->setActiveCurve(k);
    }
    updateFilesList();
    const size_t K=data->getActiveCurve();

    if (data->get_odata(K)->rows()>1)
        plotarea->setCurrentIndex(1);
    else
        plotarea->setCurrentIndex(0); //switch 1D/2D view
    if (flags&OpenFullRestart) {
            plot2D->cold_restart(data);
        plot3D->cold_restart(data);
    }
    else
        activePlot()->update();
        */
}


void ESsistMe_MainWindow::fileExit()
{
    close();
}



void ESsistMe_MainWindow::readSettings()
{
    QSettings settings(QSettings::IniFormat , QSettings::UserScope, PACKAGE_NAME, PROJECT_NAME "V" PROJECT_VERSION);
    if (settings.allKeys().isEmpty())
        return; // A virgin startup
        settings.beginGroup("MainWindow");
    restoreState(settings.value("geometry").toByteArray());
        resize(settings.value("size", QSize(1024, 768)).toSize());
        move(settings.value("pos", QPoint(0, 0)).toPoint());
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
        settings.endGroup();
}

void ESsistMe_MainWindow::writeSettings()
{
    QSettings settings(QSettings::IniFormat , QSettings::UserScope, PACKAGE_NAME, PROJECT_NAME "V" PROJECT_VERSION);
        settings.beginGroup("MainWindow");
        settings.setValue("size", size());
        settings.setValue("pos", pos());
/*	settings.setValue("dir", working_directory);
    settings.setValue("geometry", saveState());
    settings.setValue("NoiseFactor", ppSpinBox->value());
    settings.setValue("levels", levelsSpinBox->value());
    settings.setValue("posneg", posnegComboBox->currentIndex());
    settings.setValue("floor", floorSpinBox->value());
    settings.setValue("ceiling", ceilingSpinBox->value());
    settings.setValue("multiplier", multiplierSpinBox->value());
    settings.setValue("slice", sliceComboBox->currentIndex());
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
//	useHypercomplex=settings.value("UseHypercomplex",false).toBool();
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
    settings.setValue("Antialiasing",antialiasing);
    settings.setValue("CustomPrintLogo",customPrintLogo);
    settings.setValue("PrintPars",printPars);
    settings.setValue("PrintLineWidth",printLineWidth);
    settings.setValue("SmoothTransformations",smoothTransformations);
    settings.setValue("AnimatedMarkers",animatedMarkers);
    settings.setValue("Style",style);
    settings.setValue("UseBold1D",useBold1D);
    settings.setValue("UseSkyline",useSkyline);
    settings.setValue("MixFIDandSpectra",mixFidSpec);
    settings.setValue("UseSystemPrintPars",useSystemPrintPars);
        settings.setValue("Print1DMode",print1Dmode);
//	settings.setValue("UseHypercomplex",useHypercomplex);
//	settings.setValue("ProcessingToolBar",procToolBarList);
    settings.setValue("YLeftLabels",yLeftLabels);
        settings.endGroup();
}
