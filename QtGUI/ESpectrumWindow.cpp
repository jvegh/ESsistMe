#include "ESsistMe_mainwindow.h"
#include "ESpectrumWindow.h"
#include "ui_ESsistMe_mainform.h"
#include <QtCore>
#include "qcustomplot.h" // the header file of QCustomPlot.

class EResidualWindow;
class ENavigatorWindow;

ESsistMe_SpectrumWindow::ESsistMe_SpectrumWindow(QWidget *parent) :
    QCustomPlot(parent)
{
 //    resize(1000, 800);
    setupSincScatterDemo(this);
}

ESsistMe_SpectrumWindow::~ESsistMe_SpectrumWindow()
{
 //   delete ui;
}

extern  QVector<double> X0, Y0;
extern QVector<double> YConfUpper, YConfLower;
extern QVector<double> X1, Y1, Y1err, YResidual;


void ESsistMe_SpectrumWindow::setupSincScatterDemo(QCustomPlot *customPlot)
{
    legend->setVisible(true);
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
  for (int i=0; i<250; ++i)
  {
    X0[i] = (i/249.0-0.5)*30+0.01; // by adding a small offset we make sure not do divide by zero in next code line
    Y0[i] = qSin(X0[i])/X0[i] +.6; // sinc function
    YConfUpper[i] = Y0[i]+0.15;
    YConfLower[i] = Y0[i]-0.15;
    X0[i] *= 1000;
  }
  for (int i=0; i<50; ++i)
  {
    // generate a gaussian distributed random number:
    double tmp1 = rand()/(double)RAND_MAX;
    double tmp2 = rand()/(double)RAND_MAX;
    double r = qSqrt(-2*qLn(tmp1))*qCos(2*M_PI*tmp2); // box-muller transform for gaussian distribution
    // set y1 to value of y0 plus a random gaussian pertubation:
    X1[i] = (i/50.0-0.5)*30+0.25;
    Y1[i] = qSin(X1[i])/X1[i]+r*0.15 +.6;
    X1[i] *= 1000;
    Y1err[i] = 0.15;
    YResidual[i] = (Y1[i]-Y0[i])/sqrt(Y1[i]);
  }

  // pass data to graphs and let QCustomPlot determine the axes ranges so the whole thing is visible:
  customPlot->graph(0)->setData(X0, YConfUpper);
  customPlot->graph(1)->setData(X0, YConfLower);
  customPlot->graph(2)->setData(X0, Y0);
  customPlot->graph(3)->setData(X1, Y1);
  errorBars->setData(Y1err);
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
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

