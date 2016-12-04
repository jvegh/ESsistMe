#ifndef ESsistMe_MAINWINDOW_H
#define ESsistMe_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class ESsistMe_MainWindow;
}

class ESsistMe_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESsistMe_MainWindow(QWidget *parent = 0);
    ~ESsistMe_MainWindow();

private:
    Ui::ESsistMe_MainWindow *ui;
};

#endif // ESsistMe_MAINWINDOW_H
