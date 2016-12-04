/********************************************************************************
** Form generated from reading UI file 'ESsistMe_mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ESsistMe_MAINWINDOW_H
#define UI_ESsistMe_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ESsistMe_MainWindow
{
public:
    QAction *actionLoad;
    QAction *actionPrint;
    QAction *actionExit;
    QWidget *centralWidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *ControlColumn;
    QToolBox *Operations;
    QWidget *page;
    QWidget *page_2;
    QTableView *Summary;
    QTreeView *Navigation;
    QWidget *widget1;
    QVBoxLayout *SpectrumColumn;
    QPlainTextEdit *SpectrumView;
    QPlainTextEdit *ResidualView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ESsistMe_MainWindow)
    {
        if (ESsistMe_MainWindow->objectName().isEmpty())
            ESsistMe_MainWindow->setObjectName(QStringLiteral("ESsistMe main window"));
        ESsistMe_MainWindow->resize(911, 795);
        actionLoad = new QAction(ESsistMe_MainWindow);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionPrint = new QAction(ESsistMe_MainWindow);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        actionExit = new QAction(ESsistMe_MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(ESsistMe_MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(160, 10, 501, 394));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        ControlColumn = new QVBoxLayout();
        ControlColumn->setSpacing(6);
        ControlColumn->setObjectName(QStringLiteral("ControlColumn"));
        Operations = new QToolBox(widget);
        Operations->setObjectName(QStringLiteral("Operations"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 497, 328));
        Operations->addItem(page, QStringLiteral("Page 1"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 497, 328));
        Summary = new QTableView(page_2);
        Summary->setObjectName(QStringLiteral("Summary"));
        Summary->setGeometry(QRect(0, 140, 256, 227));
        Navigation = new QTreeView(page_2);
        Navigation->setObjectName(QStringLiteral("Navigation"));
        Navigation->setGeometry(QRect(0, 340, 256, 200));
        Navigation->setMinimumSize(QSize(100, 20));
        Navigation->setMaximumSize(QSize(500, 200));
        widget1 = new QWidget(page_2);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(250, -60, 258, 392));
        SpectrumColumn = new QVBoxLayout(widget1);
        SpectrumColumn->setSpacing(6);
        SpectrumColumn->setContentsMargins(11, 11, 11, 11);
        SpectrumColumn->setObjectName(QStringLiteral("SpectrumColumn"));
        SpectrumColumn->setContentsMargins(0, 0, 0, 0);
        SpectrumView = new QPlainTextEdit(widget1);
        SpectrumView->setObjectName(QStringLiteral("SpectrumView"));

        SpectrumColumn->addWidget(SpectrumView);

        ResidualView = new QPlainTextEdit(widget1);
        ResidualView->setObjectName(QStringLiteral("ResidualView"));

        SpectrumColumn->addWidget(ResidualView);

        Operations->addItem(page_2, QStringLiteral("Page 2"));

        ControlColumn->addWidget(Operations);


        horizontalLayout->addLayout(ControlColumn);

        ESsistMe_MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ESsistMe_MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 911, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        ESsistMe_MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ESsistMe_MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ESsistMe_MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ESsistMe_MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ESsistMe_MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionPrint);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);

        retranslateUi(ESsistMe_MainWindow);

        Operations->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ESsistMe_MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ESsistMe_MainWindow)
    {
        ESsistMe_MainWindow->setWindowTitle(QApplication::translate("ESsistMe_MainWindow", "ESsistMe_MainWindow", 0));
        actionLoad->setText(QApplication::translate("ESsistMe_MainWindow", "Load ...", 0));
        actionPrint->setText(QApplication::translate("ESsistMe_MainWindow", "Print", 0));
        actionExit->setText(QApplication::translate("ESsistMe_MainWindow", "Exit", 0));
        Operations->setItemText(Operations->indexOf(page), QApplication::translate("ESsistMe_MainWindow", "Page 1", 0));
        Operations->setItemText(Operations->indexOf(page_2), QApplication::translate("ESsistMe_MainWindow", "Page 2", 0));
        menuFile->setTitle(QApplication::translate("ESsistMe_MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class ESsistMe_MainWindowMainWindow: public Ui_ESsistMe_MainWindowMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ESsistMe_MAINWINDOW_H
