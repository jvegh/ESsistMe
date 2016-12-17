#ifndef ESsistMe_SPECTRUMWINDOW_H
#define ESsistMe_SPECTRUMWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "ESsistMe_mainwindow.h"

class ESsistMe_MainWindow;

class ESsistMe_SpectrumWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESsistMe_SpectrumWindow(QCustomPlot *plotter, QWidget *parent);
    ~ESsistMe_SpectrumWindow();

    void setupSincScatterDemo(QCustomPlot *customPlot);

private:
//    Ui::ESsistMe_SpectrumWindow *ui;
    QCustomPlot* spectrumPlot;
};


#endif // ESsistMe_SPECTRUMWINDOW_H
