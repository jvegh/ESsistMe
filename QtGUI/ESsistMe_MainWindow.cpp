#include "ESsistMe_mainwindow.h"
#include "ui_ESsistMe_mainform.h"
#include <QMdiSubWindow>
#include <QtCore>
#include <QFileSystemModel>
#include "qcustomplot.h" // the header file of QCustomPlot.
#include "QStuff.h"
#include "Stuff.h"
#include "FitDialog.h"

QVector<double> X0(250), Y0(250);QVector<double> YConfUpper(250), YConfLower(250);
QVector<double> X1(50), Y1(50), Y1err(50), YResidual(50);

extern struct SystemDirectories Directories;

ESsistMe_MainWindow::ESsistMe_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ESsistMe_MainWindow)
{
    ui->setupUi(this);
    // This provides synchronization between spectrum window and residual window
    connect( ui->spectrumWindow->xAxis, SIGNAL( rangeChanged(QCPRange) ), ui->residualWindow, SLOT( setRange(QCPRange) ) );
    connect( ui->spectrumWindow->xAxis, SIGNAL( rangeChanged(QCPRange) ), ui->navigatorWindow, SLOT( setLupe(QCPRange) ) );
    connect( ui->spectrumWindow->yAxis, SIGNAL( rangeChanged(QCPRange) ), ui->navigatorWindow, SLOT( setLupe(QCPRange) ) );
    (void)statusBar();
    setWindowTitle( "ESsistMe" );
    setIconSize(QSize(24,24));
    setWindowIcon( QPixmap( ":/images/ESlogo.png" ) );
    SetFileMenu();
    SetProcessMenu();
//    initFileFilterList();
    SetupSystemDirectories(this); // Establish system and user directories
    readSettings(); // Read window-related settings
    setupToolBoxes();   // Set up the tool box contents
    ESsistMe_FitDialog = new FitDialog();
}

ESsistMe_MainWindow::~ESsistMe_MainWindow()
{
    delete ui;
    delete ESsistMe_FitDialog;
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

void ESsistMe_MainWindow::addTreeRoot(QString name, QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
/* !!    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);
    addTreeChild(treeItem, name + "A", "Child_first");
    addTreeChild(treeItem, name + "B", "Child_second");
    */
}

void ESsistMe_MainWindow::addTreeChild(QTreeWidgetItem *parent,
                  QString name, QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
}

void ESsistMe_MainWindow::setupToolBoxes(void)
{
 /*   QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(model->index(QString('/')));
    ui->dirTreeView->setModel(model);
    ui->dirTreeView->setRootIndex(QString(Directories.Home.c_str()));
*/
    QFileSystemModel *dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(QString(Directories.Home.c_str()));

    ui->dirTreeView->setModel(dirModel);
    ui->dirTreeView->setRootIndex(dirModel->setRootPath(Directories.UserData.c_str()+'/'));
}
