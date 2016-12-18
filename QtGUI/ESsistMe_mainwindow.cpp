#include "ESsistMe_mainwindow.h"
#include "ui_ESsistMe_mainform.h"
#include <QMdiSubWindow>
#include <QtCore>
#include "qcustomplot.h" // the header file of QCustomPlot.

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
    static int wcounter=1;
/*	PlotArea* temparea=new PlotArea(this);
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
        event->accept();
/*      LOG(INFO) << "User terminated simulation";
    CloseSystemFiles(GetAppName() + " simulator GUI version has terminated");
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

/*
void setupSincScatterDemo(QCustomPlot *customPlot)
{
//  demoName = "Sinc Scatter Demo";
  customPlot->legend->setVisible(true);
  customPlot->legend->setFont(QFont("Helvetica",9));
  // set locale to english, so we get english decimal separator:
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  // add confidence band graphs:
  customPlot->addGraph();
  QPen pen;
  pen.setStyle(Qt::DotLine);
  pen.setWidth(1);
  pen.setColor(QColor(180,180,180));
  customPlot->graph(0)->setName("Confidence Band 68%");
  customPlot->graph(0)->setPen(pen);
  customPlot->graph(0)->setBrush(QBrush(QColor(255,50,30,20)));
  customPlot->addGraph();
  customPlot->legend->removeItem(customPlot->legend->itemCount()-1); // don't show two confidence band graphs in legend
  customPlot->graph(1)->setPen(pen);
  customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));
  // add theory curve graph:
  customPlot->addGraph();
  pen.setStyle(Qt::DashLine);
  pen.setWidth(2);
  pen.setColor(Qt::red);
  customPlot->graph(2)->setPen(pen);
  customPlot->graph(2)->setName("Theory Curve");
  // add data point graph:
  customPlot->addGraph();
  customPlot->graph(3)->setPen(QPen(Qt::blue));
  customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4));
  // add error bars:
  QCPErrorBars *errorBars = new QCPErrorBars(customPlot->xAxis, customPlot->yAxis);
  errorBars->removeFromLegend();
  errorBars->setAntialiased(false);
  errorBars->setDataPlottable(customPlot->graph(3));
  errorBars->setPen(QPen(QColor(180,180,180)));
  customPlot->graph(3)->setName("Measurement");

  // generate ideal sinc curve data and some randomly perturbed data for scatter plot:
  QVector<double> x0(250), y0(250);
  QVector<double> yConfUpper(250), yConfLower(250);
  for (int i=0; i<250; ++i)
  {
    x0[i] = (i/249.0-0.5)*30+0.01; // by adding a small offset we make sure not do divide by zero in next code line
    y0[i] = qSin(x0[i])/x0[i]; // sinc function
    yConfUpper[i] = y0[i]+0.15;
    yConfLower[i] = y0[i]-0.15;
    x0[i] *= 1000;
  }
  QVector<double> x1(50), y1(50), y1err(50);
  for (int i=0; i<50; ++i)
  {
    // generate a gaussian distributed random number:
    double tmp1 = rand()/(double)RAND_MAX;
    double tmp2 = rand()/(double)RAND_MAX;
    double r = qSqrt(-2*qLn(tmp1))*qCos(2*M_PI*tmp2); // box-muller transform for gaussian distribution
    // set y1 to value of y0 plus a random gaussian pertubation:
    x1[i] = (i/50.0-0.5)*30+0.25;
    y1[i] = qSin(x1[i])/x1[i]+r*0.15;
    x1[i] *= 1000;
    y1err[i] = 0.15;
   }
  // pass data to graphs and let QCustomPlot determine the axes ranges so the whole thing is visible:
  customPlot->graph(0)->setData(x0, yConfUpper);
  customPlot->graph(1)->setData(x0, yConfLower);
  customPlot->graph(2)->setData(x0, y0);
  customPlot->graph(3)->setData(x1, y1);
  errorBars->setData(y1err);
  customPlot->graph(2)->rescaleAxes();
  customPlot->graph(3)->rescaleAxes(true);
  // setup look of bottom tick labels:
  customPlot->xAxis->setTickLabelRotation(30);
  customPlot->xAxis->ticker()->setTickCount(9);
  customPlot->xAxis->setNumberFormat("ebc");
  customPlot->xAxis->setNumberPrecision(1);
  customPlot->xAxis->moveRange(-10);
  // make top right axes clones of bottom left axes. Looks prettier:
  customPlot->axisRect()->setupFullAxesBox();
}
*/
