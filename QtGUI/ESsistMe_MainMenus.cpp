#include "ESsistMe_mainwindow.h"
#include "ui_ESsistMe_mainform.h"
#include <QMdiSubWindow>
#include <QtCore>
#include <QFileSystemModel>
#include "qcustomplot.h" // the header file of QCustomPlot.
#include "QStuff.h"
#include "Stuff.h"

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

void ESsistMe_MainWindow::SetProcessMenu(void)
{
processFitAction = new QAction( this);
processFitAction->setText( tr( "&Fit" ) );
processFitAction->setIcon( QIcon( ":/images/window_new.png") );
//processFitAction->setShortcut(QKeySequence::New);
//processFitAction->setShortcuts(QKeySequence::New);
processFitAction->setStatusTip(tr("Fit the spectrum"));
connect( processFitAction, SIGNAL( triggered() ), this, SLOT( ESsistMe_MainWindow::processFit() ) );

ui->processMenu = ui->menubar->addMenu(tr("&Process"));
ui->processMenu->addAction(processFitAction);
ui->processMenu->addSeparator();
}



void ESsistMe_MainWindow::fileNewWindow()
{
    ESsistMe_FitDialog->show();
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

void ESsistMe_MainWindow::processFit()
{
    qDebug() << "Fit process invoked";
    FitDialog w;
    w.show();
    qDebug() << "Fit process returned";
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


