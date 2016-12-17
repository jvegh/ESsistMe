/********************************************************************************
** Form generated from reading UI file 'mainform.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ESsistMe_MAINFORM_H
#define UI_ESsistMe_MAINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSplitter>
#include "qcustomplot.h"
#include "ESpectrumWindow.h"
#include "ENavigatorWindow.h"

QT_BEGIN_NAMESPACE

class Ui_ESsistMe_MainWindow
{
public:
    QWidget *centralwidget, *spectrumWidget;
//    QVBoxLayout *vboxLayout;
    ESsistMe_SpectrumWindow* spectrumWindow;
    ESsistMe_NavigatorWindow* navigatorWindow;
    QMdiArea
    *mdiAreaRes;
    QCustomPlot *spectrumPlot, *navigatorPlot;
//    QDockWidget *infoDockWidget;
//    QWidget *dockWidgetContents_2;
//    QVBoxLayout *vboxLayout1;
    QTextEdit *infoTextEdit;
    QTreeView *dirTreeView;
    QDockWidget *filesDockWidget;
//    QWidget *dockWidgetContents_4;
//    QVBoxLayout *vboxLayout7;
    QListWidget *filesListWidget;
    QToolBox *toolBox;
    QHBoxLayout *topLayout;
    QSplitter *spectrumsplitter, *controlsplitter, *mainsplitter;
    
    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("ESsistMe_MainWindow"));
        MainWindow->resize(1063, 798);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        topLayout = new QHBoxLayout(centralwidget);

    // Add global layout with two embedded splitters
        MainWindow->setCentralWidget(centralwidget);
	mainsplitter = new QSplitter(centralwidget);	
        topLayout->addWidget(mainsplitter);
     // Create embedded splitters
        spectrumsplitter = new QSplitter(centralwidget);
        spectrumsplitter->setOrientation(Qt::Vertical);
        controlsplitter = new QSplitter(centralwidget);
        controlsplitter->setOrientation(Qt::Vertical);
        mainsplitter->addWidget(spectrumsplitter);
        mainsplitter->addWidget(controlsplitter);
    // Load embedded splitters
        // The spectrum splitter: a fit window
        spectrumPlot = new QCustomPlot(spectrumsplitter);
        spectrumPlot->setObjectName(QStringLiteral("customPlot"));
        spectrumWindow = new ESsistMe_SpectrumWindow(spectrumPlot, centralwidget);
        spectrumPlot->resize(500,300);
        spectrumsplitter->addWidget(spectrumWindow);
        // and a residual window
        mdiAreaRes = new QMdiArea(centralwidget);
        mdiAreaRes->setObjectName(QStringLiteral("mdiAreaFit"));
        mdiAreaRes->setObjectName(QStringLiteral("mdiAreaRes"));
    //    mdisplitter->addWidget(mdiAreaFit);
        spectrumsplitter->addWidget(mdiAreaRes);

        // The control splitter
        toolBox = new QToolBox;
	toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
        controlsplitter->addWidget(toolBox);
 //       spectrumPlot->setGeometry(spectrumWindow->geometry());
        // The navigator vindow
        navigatorPlot = new QCustomPlot(controlsplitter);
        navigatorPlot->setObjectName(QStringLiteral("customPlot"));
        navigatorWindow = new ESsistMe_NavigatorWindow(navigatorPlot, centralwidget);
        navigatorPlot->resize(300,200);
        controlsplitter->addWidget(navigatorWindow);

        // Set up now the infoTextEdit
        infoTextEdit = new QTextEdit(centralwidget);
        infoTextEdit->setObjectName(QStringLiteral("infoTextEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(infoTextEdit->sizePolicy().hasHeightForWidth());
        infoTextEdit->setSizePolicy(sizePolicy);
        infoTextEdit->setMaximumSize(QSize(99999, 99999));
        infoTextEdit->setUndoRedoEnabled(false);
        infoTextEdit->setReadOnly(true);
        controlsplitter->addWidget(infoTextEdit);
 
        QFont font1;
        font1.setPointSize(8);
/*      hboxLayout2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout2->setSpacing(6);
#endif
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QStringLiteral("hboxLayout2"));
        */
        dirTreeView = new QTreeView(centralwidget);
        dirTreeView->setObjectName(QStringLiteral("dirTreeView"));

	toolBox->addItem(dirTreeView, "DirTree");

 /*       filesDockWidget = new QDockWidget(MainWindow);
        filesDockWidget->setObjectName(QStringLiteral("filesDockWidget"));
        filesDockWidget->setFloating(false);
        filesDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
 */       filesListWidget = new QListWidget(centralwidget);
        filesListWidget->setObjectName(QStringLiteral("filesListWidget"));
        sizePolicy.setHeightForWidth(filesListWidget->sizePolicy().hasHeightForWidth());
        filesListWidget->setSizePolicy(sizePolicy);
        filesListWidget->setMinimumSize(QSize(0, 0));
        filesListWidget->setMaximumSize(QSize(999999, 999999));
        filesListWidget->setFont(font1);
        filesListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        filesListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

 	toolBox->addItem(filesListWidget, "FileTree");
    } // setupUi


};

namespace Ui {
    class ESsistMe_MainWindow: public Ui_ESsistMe_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ESsistMe_MAINFORM_H
