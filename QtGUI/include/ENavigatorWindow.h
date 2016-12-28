#ifndef ESsistMe_NAVIGATORWINDOW_H
#define ESsistMe_NAVIGATORWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "ESsistMe_mainwindow.h"

class ESsistMe_MainWindow;

class ESsistMe_NavigatorWindow : public QCustomPlot
{
    Q_OBJECT

public:
    explicit ESsistMe_NavigatorWindow( QWidget *parent);
    ~ESsistMe_NavigatorWindow();

    void setupSincScatterDemo(QCustomPlot *customPlot);
public slots:
    void setLupe(const QCPRange &range);

private:
 //   QCustomPlot* navigatorPlot;
};



#endif // ESsistMe_NAVIGATORWINDOW_H
