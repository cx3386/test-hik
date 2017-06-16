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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "cqtopencvviewergl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *action_Start;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *checkTab;
    QGridLayout *gridLayout_2;
    CQtOpenCVViewerGl *checkViewer;
    QWidget *realPlayTab;
    QGridLayout *gridLayout_3;
    QOpenGLWidget *realPlayViewer;
    QMenuBar *menuBar;
    QMenu *menu_Tools;
    QMenu *menuCamera;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(1086, 724);
        action_Start = new QAction(MainWindowClass);
        action_Start->setObjectName(QStringLiteral("action_Start"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        checkTab = new QWidget();
        checkTab->setObjectName(QStringLiteral("checkTab"));
        gridLayout_2 = new QGridLayout(checkTab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        checkViewer = new CQtOpenCVViewerGl(checkTab);
        checkViewer->setObjectName(QStringLiteral("checkViewer"));

        gridLayout_2->addWidget(checkViewer, 0, 0, 1, 1);

        tabWidget->addTab(checkTab, QString());
        realPlayTab = new QWidget();
        realPlayTab->setObjectName(QStringLiteral("realPlayTab"));
        gridLayout_3 = new QGridLayout(realPlayTab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        realPlayViewer = new QOpenGLWidget(realPlayTab);
        realPlayViewer->setObjectName(QStringLiteral("realPlayViewer"));

        gridLayout_3->addWidget(realPlayViewer, 0, 0, 1, 1);

        tabWidget->addTab(realPlayTab, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1086, 23));
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

        retranslateUi(MainWindowClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", Q_NULLPTR));
        action_Start->setText(QApplication::translate("MainWindowClass", "&Start", Q_NULLPTR));
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
