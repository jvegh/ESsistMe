#ifndef ESsistMe_MAINWINDOW_H
#define ESsistMe_MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
//#include "filefilters.h"
#include "qcustomplot.h"
#include "FitDialog.h"

namespace Ui {
class ESsistMe_MainWindow;
}

/*!
 * \brief The Overall plot sytyle structure, as used by QCustomPlot
 *
 * All subwindows have their own plot styles, only the structure is the same
 */
struct PlotStyle
{
    bool ConfidenceBands_Present;   // If to draw component in question
    bool Legend_Present;
    bool TheoryCurve_Present;
    bool MeasurementData_Present;
    bool MeasurementError_Present;
    Qt::PenStyle ConfidenceBands_PenStyle; // The style of line drawing
    Qt::PenStyle TheoryCurve_PenStyle;
    QColor ConfidenceBands_PenColor;
    QColor MeasurementData_PenColor;
    QColor TheoryCurve_PenColor;
    QCPGraph::LineStyle MeasurementData_LineStyle;
    QCPScatterStyle  TheoryCurve_ScatterShape;
    QCPScatterStyle  MeasurementData_ScatterShape;

    QColor ConfidenceBands_BrushColor;
    short int ConfidenceBands_LineWidth; // The width of the line
    short int TheoryCurve_LineWidth;

    Qt::PenStyle Theory_LineStyle; // The style of line drawing

};


class ESsistMe_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESsistMe_MainWindow(QWidget *parent = 0);
    ~ESsistMe_MainWindow();
    QToolBar *toolBar;
    QToolBar * plotToolBar;
    QToolBar * procToolBar;
    QAction *fileNewWindowAction, *fileOpenAction, *fileExitAction;
    QAction *processFitAction;
//    QAction* fileSaveAction;
    QMenuBar *menubar;
    QMenu *fileMenu, *processMenu;
//    QList<BaseFileFilter* > fileFilterList;
//    void initFileFilterList();
     void readSettings();
     void writeSettings();
     void setupToolBoxes(void);

protected:
        void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void fileNewWindow();
    void fileOpenDialog();
    virtual void fileExit();
    virtual void processFit();
private:
    Ui::ESsistMe_MainWindow *ui;
    void SetFileMenu(void);
    void SetProcessMenu(void);
    void fileOpen(QString name,  QString filter, int flags);
    QString working_directory; //contains working directory
    bool maybeClose();
    FitDialog *ESsistMe_FitDialog;

    void addTreeRoot(QString name, QString description);
    void addTreeChild(QTreeWidgetItem *parent,
                      QString name, QString description);

};



enum {UNITS_MEV, UNITS_EV, UNITS_KEV, UNITS_COUNTS, UNITS_CPS
    //UNITS_HZ, UNITS_KHZ, UNITS_PPM, UNITS_US, UNITS_MS, UNITS_S, UNITS_FPTS, UNITS_TPTS, UNITS_TVIRT, UNITS_FVIRT
    , UNITS_NONE};

enum {PRINT1D_ALL, PRINT1D_SKIP, PRINT1D_SPLIT};

class global_options_ {
public:
        size_t ticks;
        bool hasXaxis;
        bool hasYaxis;
        bool hasExtraTicks;
        size_t extraTicks;
        QString axisFont;
        bool antialiasing;
        bool smoothTransformations;
        bool animatedMarkers;
        QString customPrintLogo;
        bool printPars;
        int printLineWidth;
        int print1Dmode; //PRINT1D_ALL-print single line; PRINT1D_SKIP-reduce resolution; PRINT1D_SPLIT-split
        QString style;
        int xunits,  yunits;
//	bool useHypercomplex;
        bool useBold1D;
        bool mixFidSpec;
        bool useSkyline;
        bool useSystemPrintPars;
        bool yLeftLabels; //print labels on the left from y-axis
//	QStringList procToolBarList;
//	size_t maxAttachedProc; //maximum of remebered files with attached processing
        global_options_() {readSettings();//maxAttachedProc=100;
                           useSystemPrintPars=true;};
        ~global_options_() {writeSettings();};
private:
        void readSettings();
        void writeSettings();
};

#endif // ESsistMe_MAINWINDOW_H
