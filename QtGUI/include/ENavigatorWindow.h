#ifndef ESsistMe_NAVIGATORWINDOW_H
#define ESsistMe_NAVIGATORWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "ESsistMe_mainwindow.h"

class ESsistMe_MainWindow;

class ESsistMe_NavigatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESsistMe_NavigatorWindow(QCustomPlot *plotter, QWidget *parent);
    ~ESsistMe_NavigatorWindow();

    void setupSincScatterDemo(QCustomPlot *customPlot);

private:
    QCustomPlot* navigatorPlot;
};



#endif // ESsistMe_NAVIGATORWINDOW_H
