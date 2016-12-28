#include "ESsistMe_mainwindow.h"
#include "ENavigatorWindow.h"
#include "ui_ESsistMe_mainform.h"
#include <QtCore>
#include "qcustomplot.h" // the header file of QCustomPlot.

extern  QVector<double> X0, Y0;
extern QVector<double> YConfUpper, YConfLower;
extern QVector<double> X1, Y1, Y1err;


ESsistMe_NavigatorWindow::ESsistMe_NavigatorWindow(QWidget *parent) :
    QCustomPlot(parent)
{
//    navigatorPlot = plotter;
//    resize(300, 150);
    setupSincScatterDemo(this);
}

ESsistMe_NavigatorWindow::~ESsistMe_NavigatorWindow()
{
 //   delete ui;
}

void ESsistMe_NavigatorWindow::
setLupe(const QCPRange &range)
{
xAxis->setRange(range); replot();
}

void ESsistMe_NavigatorWindow::setupSincScatterDemo(QCustomPlot *customPlot)
{
    //  demoName = "Sinc Scatter Demo";
    //  customPlot->legend->setVisible(true);
    //  customPlot->legend->setFont(QFont("Helvetica",9));
      // set locale to english, so we get english decimal separator:
      customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
      // add data point graph:
      customPlot->addGraph();
      customPlot->graph(0)->setPen(QPen(Qt::blue));
      customPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);

      // pass data to graphs and let QCustomPlot determine the axes ranges so the whole thing is visible:
      customPlot->graph(0)->setData(X1, Y1);
      customPlot->graph(0)->rescaleAxes();
      // make top right axes clones of bottom left axes. Looks prettier:
 //     customPlot->axisRect()->setupFullAxesBox(false);
}
