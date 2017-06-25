/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *action_Start;
    QAction *action_Stop;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QTabWidget *tabWidget;
    QWidget *checkTab;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView1;
    QTextBrowser *logBrowser;
    QGraphicsView *graphicsView2;
    QWidget *realPlayTab;
    QMenuBar *menuBar;
    QMenu *menu_Tools;
    QMenu *menuCamera;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(627, 486);
        action_Start = new QAction(MainWindowClass);
        action_Start->setObjectName(QStringLiteral("action_Start"));
        action_Stop = new QAction(MainWindowClass);
        action_Stop->setObjectName(QStringLiteral("action_Stop"));
        action_Stop->setEnabled(false);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        checkTab = new QWidget();
        checkTab->setObjectName(QStringLiteral("checkTab"));
        gridLayout = new QGridLayout(checkTab);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        graphicsView1 = new QGraphicsView(checkTab);
        graphicsView1->setObjectName(QStringLiteral("graphicsView1"));
        graphicsView1->setMinimumSize(QSize(192, 108));

        gridLayout->addWidget(graphicsView1, 0, 0, 1, 1);

        logBrowser = new QTextBrowser(checkTab);
        logBrowser->setObjectName(QStringLiteral("logBrowser"));

        gridLayout->addWidget(logBrowser, 0, 1, 2, 1);

        graphicsView2 = new QGraphicsView(checkTab);
        graphicsView2->setObjectName(QStringLiteral("graphicsView2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView2->sizePolicy().hasHeightForWidth());
        graphicsView2->setSizePolicy(sizePolicy);
        graphicsView2->setMinimumSize(QSize(192, 108));

        gridLayout->addWidget(graphicsView2, 1, 0, 1, 1);

        tabWidget->addTab(checkTab, QString());
        realPlayTab = new QWidget();
        realPlayTab->setObjectName(QStringLiteral("realPlayTab"));
        tabWidget->addTab(realPlayTab, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 627, 30));
        menu_Tools = new QMenu(menuBar);
        menu_Tools->setObjectName(QStringLiteral("menu_Tools"));
        menuCamera = new QMenu(menu_Tools);
        menuCamera->setObjectName(QStringLiteral("menuCamera"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menu_Tools->menuAction());
        menu_Tools->addAction(menuCamera->menuAction());
        menuCamera->addAction(action_Start);
        menuCamera->addAction(action_Stop);

        retranslateUi(MainWindowClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", Q_NULLPTR));
        action_Start->setText(QApplication::translate("MainWindowClass", "&Start", Q_NULLPTR));
        action_Stop->setText(QApplication::translate("MainWindowClass", "&Stop", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(checkTab), QApplication::translate("MainWindowClass", "Check", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(realPlayTab), QApplication::translate("MainWindowClass", "Real Play", Q_NULLPTR));
        menu_Tools->setTitle(QApplication::translate("MainWindowClass", "&Tools", Q_NULLPTR));
        menuCamera->setTitle(QApplication::translate("MainWindowClass", "&Camera", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
