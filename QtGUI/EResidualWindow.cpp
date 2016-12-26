#include "ESsistMe_mainwindow.h"
#include "EResidualWindow.h"
#include "ui_ESsistMe_mainform.h"
#include <QtCore>
#include "qcustomplot.h" // the header file of QCustomPlot.

extern  QVector<double> X0, Y0;
extern QVector<double> YConfUpper, YConfLower;
extern QVector<double> X1, Y1, Y1err, YResidual;

ESsistMe_ResidualWindow::ESsistMe_ResidualWindow( QWidget *parent) :
    QCustomPlot(parent)
{
    setupSimpleDemo(this);
}

ESsistMe_ResidualWindow::~ESsistMe_ResidualWindow()
{
 //   delete ui;
}
int NDist = 22; double Range = 1.5; double DRange = NDist/2./Range; double MRange = -Range;
double Rounding = Range/NDist;

QVector<double> RGraph(NDist+2,-0.0L); QVector<double> RGraphX(NDist+2);


int MyIndex(double Val)
{
    return Val < MRange ? 0 : Val>Range ? NDist+1 : (Val-MRange)*DRange + .99999;
}

void ESsistMe_ResidualWindow::setupSimpleDemo(QCustomPlot *customPlot)
{
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  customPlot->addGraph();
  customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
  customPlot->graph(0)->setData(X1,  YResidual);
  customPlot->graph(0)->rescaleAxes();
  QCPRange MyRange = customPlot->xAxis->range();
  customPlot->yAxis->setRange(MRange-Rounding, Range+Rounding);
  customPlot->yAxis2->setRange(MRange-Rounding, Range+Rounding);
  // Here we have two scales
  // for the two graphs
//  customPlot->graph(0)->rescaleAxes();
  double CellWidth = 2*Range/NDist;
    for(int i = 0; i<NDist+2; i++)
    {
        RGraphX[i] = MRange + (i-1)*CellWidth;
    }
    double MyMax = 0;
  for (int i=0; i<50; ++i)
  {

    int j =  MyIndex(YResidual[i]);
    RGraph[j] += 500.0L;
    if(RGraph[j] > MyMax) MyMax = RGraph[j];
  }
  double norm = (MyRange.upper-MyRange.lower)/MyMax/2;
  for (int i=0; i<NDist+2; ++i)
  {

    RGraph[i] = MyRange.lower+RGraph[i]*norm;
  }

  customPlot->addGraph(yAxis,xAxis);
  customPlot->graph(1)->setLineStyle(QCPGraph::lsLine);
  customPlot->graph(1)->setPen(QPen(Qt::red));
  customPlot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20))); // first graph will be filled with translucent blue
  customPlot->graph(1)->setData(  RGraphX, RGraph);
  customPlot->xAxis2->setVisible(true);
  customPlot->yAxis2->setVisible(true);
  customPlot->yAxis2->setRange(0, MyMax);


}
