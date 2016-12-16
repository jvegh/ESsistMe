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
    explicit ESsistMe_SpectrumWindow(QCustomPlot *parent);
    ~ESsistMe_SpectrumWindow();

    void setupSincScatterDemo(QCustomPlot *customPlot);

private:
//    Ui::ESsistMe_SpectrumWindow *ui;
    QCustomPlot* spectrumPlot;
};

class ESsistMe_ResidualWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESsistMe_ResidualWindow(QCustomPlot *parent);
    ~ESsistMe_ResidualWindow();

   // void setupSincScatterDemo(QCustomPlot *customPlot);

private:
 //   Ui::ESsistMe_ResidualWindow *ui;
};

class ESsistMe_NavigatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESsistMe_NavigatorWindow(ESsistMe_MainWindow *parent);
    ~ESsistMe_NavigatorWindow();

  //  void setupSincScatterDemo(QCustomPlot *customPlot);

private:
 //   Ui::ESsistMe_NavigatorWindow *ui;
};




#endif // ESsistMe_SPECTRUMWINDOW_H
