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

#endif // ESsistMe_MAINWINDOW_H
