#ifndef ESsistMe_RESIDUALWINDOW_H
#define ESsistMe_RESIDUALWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "ESsistMe_mainwindow.h"

class ESsistMe_MainWindow;

class ESsistMe_ResidualWindow : public QCustomPlot
{
    Q_OBJECT

public:
    explicit ESsistMe_ResidualWindow(QWidget *parent);
    ~ESsistMe_ResidualWindow();

    void setupSimpleDemo(QCustomPlot *customPlot);
public slots:
    void setRange(const QCPRange &range);
private:
//    Ui::ESsistMe_SpectrumWindow *ui;
//    QCustomPlot* residualPlot;
};


#endif // ESsistMe_RESIDUALWINDOW_H
