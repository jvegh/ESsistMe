#ifndef ESsistMe_MAINWINDOW_H
#define ESsistMe_MAINWINDOW_H

#include <QMainWindow>
//#include "filefilters.h"

namespace Ui {
class ESsistMe_MainWindow;
}

class ESsistMe_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESsistMe_MainWindow(QWidget *parent = 0);
    ~ESsistMe_MainWindow();
    QToolBar *toolBar;
    QToolBar * plotToolBar;
    QToolBar * procToolBar;
    QAction* fileNewWindowAction;
    QAction* fileOpenAction;
    QAction* fileExitAction;
//    QAction* fileSaveAction;
    QMenuBar *menubar;
    QMenu *fileMenu;
//    QList<BaseFileFilter* > fileFilterList;
//    void initFileFilterList();
     void readSettings();
     void writeSettings();

protected:
        void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void fileNewWindow();
    void fileOpenDialog();
    virtual void fileExit();

private:
    Ui::ESsistMe_MainWindow *ui;
    void SetFileMenu(void);
    void fileOpen(QString name,  QString filter, int flags);
    QString working_directory; //contains working directory
    bool maybeClose();

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
