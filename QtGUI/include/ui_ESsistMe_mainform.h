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

QT_BEGIN_NAMESPACE

class Ui_ESsistMe_MainWindow
{
public:
    QWidget *centralwidget, *spectrumWidget;
//    QVBoxLayout *vboxLayout;
    ESsistMe_SpectrumWindow* spectrumWindow;
    QMdiArea
    *mdiAreaRes,*mdiAreaNav;
    QCustomPlot *spectrumPlot ;
//    QDockWidget *infoDockWidget;
//    QWidget *dockWidgetContents_2;
//    QVBoxLayout *vboxLayout1;
    QTextEdit *infoTextEdit;
//    QDockWidget *d1DockWidget;
//    QWidget *dockWidgetContents_11;
//    QVBoxLayout *vboxLayout2;
//    QVBoxLayout *vboxLayout3;
//    QDockWidget *procDockWidget;
 //   QWidget *dockWidgetContents_7;
//    QVBoxLayout *vboxLayout4;
//    QVBoxLayout *vboxLayout5;
    QTreeView *dirTreeView;
    QDockWidget *filesDockWidget;
//    QWidget *dockWidgetContents_4;
//    QVBoxLayout *vboxLayout7;
    QListWidget *filesListWidget;
    QToolBox *toolBox;
    QHBoxLayout *topLayout;
    QSplitter *mdisplitter, *mainsplitter, *stacksplitter;
    
    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("ESsistMe_MainWindow"));
        MainWindow->resize(1063, 798);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));

        MainWindow->setCentralWidget(centralwidget);

        topLayout = new QHBoxLayout(centralwidget);
	mainsplitter = new QSplitter(centralwidget);	
	toolBox = new QToolBox;
	toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
	stacksplitter = new QSplitter(centralwidget);
	stacksplitter->setOrientation(Qt::Vertical);
        stacksplitter->addWidget(toolBox);
	mdisplitter = new QSplitter(centralwidget);
	mdisplitter->setOrientation(Qt::Vertical);
  //      spectrumWidget = new QWidget(centralwidget);
        spectrumPlot = new QCustomPlot(stacksplitter);
        spectrumPlot->setObjectName(QStringLiteral("customPlot"));
        spectrumWindow = new ESsistMe_SpectrumWindow(spectrumPlot, centralwidget);
        spectrumPlot->resize(500,300);
 //       spectrumPlot->setGeometry(spectrumWindow->geometry());
        //mdiAreaFit = new QMdiArea(centralwidget);
        mdiAreaRes = new QMdiArea(centralwidget);
        mdiAreaRes->setObjectName(QStringLiteral("mdiAreaFit"));
        mdiAreaRes->setObjectName(QStringLiteral("mdiAreaRes"));
    //    mdisplitter->addWidget(mdiAreaFit);
          mdisplitter->addWidget(spectrumWindow);

        mdisplitter->addWidget(mdiAreaRes);

        mdiAreaNav = new QMdiArea(centralwidget);
        mdiAreaNav->setObjectName(QStringLiteral("mdiAreaNav"));
	mainsplitter->addWidget(mdisplitter);
	mainsplitter->addWidget(stacksplitter);
	topLayout->addWidget(mainsplitter);
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
	stacksplitter->addWidget(mdiAreaNav);
	stacksplitter->addWidget(infoTextEdit);
 
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
