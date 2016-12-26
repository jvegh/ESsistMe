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
    mPlotStyle = new PlotStyle;
    mPlotStyle->ConfidenceBands_Present = true;   // Draws two confidence bars, too
    mPlotStyle->Legend_Present = true;   // Draws two confidence bars, too
    mPlotStyle->ConfidenceBands_PenStyle = Qt::DotLine;
    mPlotStyle->ConfidenceBands_LineWidth = 1;
    mPlotStyle->ConfidenceBands_PenColor = QColor(180,180,180);

    mPlotStyle->TheoryCurve_Present = true;
    mPlotStyle->TheoryCurve_PenStyle = Qt::DashLine;
    mPlotStyle->TheoryCurve_LineWidth = 2;
    mPlotStyle->TheoryCurve_PenColor = Qt::red;
    mPlotStyle->TheoryCurve_ScatterShape = QCPScatterStyle(QCPScatterStyle::ssNone, 4);

    mPlotStyle->MeasurementData_Present = true;
    mPlotStyle->MeasurementData_PenColor = Qt::blue;
    mPlotStyle->MeasurementData_LineStyle = QCPGraph::lsNone;
    mPlotStyle->MeasurementData_ScatterShape = QCPScatterStyle(QCPScatterStyle::ssCross, 4);

    mPlotStyle->MeasurementError_Present = true;

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
    short int MyGraphNo = 0;
    if(mPlotStyle->Legend_Present)
    {
        legend->setVisible(true);
        customPlot->legend->setFont(QFont("Helvetica",9));
    }
  // set locale to english, so we get english decimal separator:
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  QPen pen;
  if(mPlotStyle->ConfidenceBands_Present)
    {
        // add confidence band graphs:
        customPlot->addGraph();
        pen.setStyle(mPlotStyle->ConfidenceBands_PenStyle);
        pen.setWidth(mPlotStyle->ConfidenceBands_LineWidth);
        pen.setColor(mPlotStyle->ConfidenceBands_PenColor);
        graph(MyGraphNo)->setName("Confidence Band 68%");
        graph(MyGraphNo)->setPen(pen);
        graph(MyGraphNo)->setBrush(QBrush(QColor(255,50,30,20)));
        addGraph();
        if(mPlotStyle->Legend_Present)
            legend->removeItem(customPlot->legend->itemCount()-1); // don't show two confidence band graphs in legend
        graph(MyGraphNo+1)->setPen(pen);
        graph(MyGraphNo)->setChannelFillGraph(customPlot->graph(MyGraphNo+1));
        MyGraphNo +=2;  // The two first graphs are the confidence bars
    }

    if(mPlotStyle->TheoryCurve_Present)
    { // add theory curve graph:
        addGraph();
        pen.setStyle(mPlotStyle->TheoryCurve_PenStyle);
        pen.setWidth(mPlotStyle->TheoryCurve_LineWidth);
        pen.setColor(mPlotStyle->TheoryCurve_PenColor);
        graph(MyGraphNo)->setScatterStyle(mPlotStyle->TheoryCurve_ScatterShape);

        customPlot->graph(MyGraphNo)->setPen(pen);
        graph(MyGraphNo)->setName("Theory Curve");
        MyGraphNo +=1;
    }
    QCPErrorBars *errorBars = new QCPErrorBars(xAxis, yAxis);

    if(mPlotStyle->MeasurementData_Present)
    {   // add data point graph:
          addGraph();
        graph(MyGraphNo)->setPen(mPlotStyle->MeasurementData_PenColor);
        graph(MyGraphNo)->setLineStyle(mPlotStyle->MeasurementData_LineStyle);
        graph(MyGraphNo)->setScatterStyle(mPlotStyle->MeasurementData_ScatterShape);
        // add error bars:
  errorBars->removeFromLegend();
  errorBars->setAntialiased(false);
  errorBars->setDataPlottable(graph(MyGraphNo));
  errorBars->setPen(QPen(QColor(180,180,180)));
  graph(MyGraphNo)->setName("Measurement");

    }



  // generate ideal sinc curve data and some randomly perturbed data for scatter plot:
  for (int i=0; i<250; ++i)
  {
    X0[i] = (i/249.0-0.5)*30+0.01; // by adding a small offset we make sure not do divide by zero in next code line
    Y0[i] = qSin(X0[i])/X0[i] +.6; // sinc function
    YConfUpper[i] = Y0[i]+0.15;
    YConfLower[i] = Y0[i]-0.15;
    X0[i] *= 1000;
  }

 // Collect residual statistics in a histogram

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
    Y1err[i] = 0.25;
    YResidual[i] = (Y1[i]-Y0[i*5])/Y1err[i];//sqrt(Y1[i]);
  }



  // pass data to graphs and let QCustomPlot determine the axes ranges so the whole thing is visible:
  customPlot->graph(0)->setData(X0, YConfUpper);
  customPlot->graph(1)->setData(X0, YConfLower);
  customPlot->graph(2)->setData(X0, Y0);
  customPlot->graph(3)->setData(X1, Y1);

//  QCPErrorBars *errorBars = new QCPErrorBars(customPlot->xAxis, customPlot->yAxis);
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

