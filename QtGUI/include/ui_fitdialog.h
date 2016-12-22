/********************************************************************************
** Form generated from reading UI file 'fitdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FITDIALOG_H
#define UI_FITDIALOG_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_FitDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QSpinBox *spinBoxIterations;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QPushButton *doTaskButton;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;

    void setupUi(QDialog *FitDialog)
    {
        if (FitDialog->objectName().isEmpty())
            FitDialog->setObjectName(QString::fromUtf8("FitDialog"));
        FitDialog->resize(311, 142);
        gridLayout = new QGridLayout(FitDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(FitDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        spinBoxIterations = new QSpinBox(FitDialog);
        spinBoxIterations->setObjectName(QString::fromUtf8("spinBoxIterations"));
        spinBoxIterations->setValue(20);

        horizontalLayout_3->addWidget(spinBoxIterations);


        gridLayout->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(FitDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        doTaskButton = new QPushButton(FitDialog);
        doTaskButton->setObjectName(QString::fromUtf8("doTaskButton"));

        horizontalLayout_2->addWidget(doTaskButton);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(FitDialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(FitDialog);
        QObject::connect(pushButton_2, SIGNAL(clicked()), FitDialog, SLOT(close()));
        QObject::connect(doTaskButton, SIGNAL(clicked()), FitDialog, SLOT(on_doFitButton_clicked()));

        QMetaObject::connectSlotsByName(FitDialog);
    } // setupUi

    void retranslateUi(QDialog *FitDialog)
    {
        FitDialog->setWindowTitle(QApplication::translate("FitDialog", "FitDialog", 0, 0));
        label_2->setText(QApplication::translate("FitDialog", "Number of iterations", 0, 0));
        label->setText(QApplication::translate("FitDialog", "Time Consuming Work", 0, 0));
        doTaskButton->setText(QApplication::translate("FitDialog", "Do Task", 0, 0));
        pushButton_2->setText(QApplication::translate("FitDialog", "Cancel", 0, 0));
    } // retranslateUi

};

namespace Ui {
    class FitDialog: public Ui_FitDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FITDIALOG_H
