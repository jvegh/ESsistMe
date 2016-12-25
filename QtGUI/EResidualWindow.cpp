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


void ESsistMe_ResidualWindow::setupSimpleDemo(QCustomPlot *customPlot)
{
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  customPlot->addGraph();
  customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
  customPlot->graph(0)->setData(X1,  YResidual);
  customPlot->graph(0)->rescaleAxes();
}
