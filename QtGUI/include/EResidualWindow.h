#ifndef ESsistMe_RESIDUALWINDOW_H
#define ESsistMe_RESIDUALWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "ESsistMe_mainwindow.h"

class ESsistMe_MainWindow;

class ESsistMe_ResidualWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESsistMe_ResidualWindow(QCustomPlot *plotter, QWidget *parent);
    ~ESsistMe_ResidualWindow();

    void setupSincScatterDemo(QCustomPlot *customPlot);

private:
//    Ui::ESsistMe_SpectrumWindow *ui;
    QCustomPlot* residualPlot;
};


#endif // ESsistMe_RESIDUALWINDOW_H
