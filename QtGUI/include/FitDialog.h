#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>

namespace Ui {
class FitDialog;
}

class FitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FitDialog(QWidget *parent = 0);
    ~FitDialog();

private slots:
    void on_doFitButton_clicked();

private:
    Ui::FitDialog *ui;
};

#endif // TASKDIALOG_H
