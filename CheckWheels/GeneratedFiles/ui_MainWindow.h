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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *action_Start;
    QAction *action_Stop;
    QAction *action_Open_ini;
    QAction *action_Load_ini;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *checkTab;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *imageLabel1;
    QLabel *imageLabel2;
    QTextBrowser *logBrowser;
    QWidget *realPlayTab;
    QMenuBar *menuBar;
    QMenu *menu_Tools;
    QMenu *menuCamera;
    QMenu *menu_Settings;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(949, 710);
        action_Start = new QAction(MainWindowClass);
        action_Start->setObjectName(QStringLiteral("action_Start"));
        action_Stop = new QAction(MainWindowClass);
        action_Stop->setObjectName(QStringLiteral("action_Stop"));
        action_Stop->setEnabled(false);
        action_Open_ini = new QAction(MainWindowClass);
        action_Open_ini->setObjectName(QStringLiteral("action_Open_ini"));
        action_Load_ini = new QAction(MainWindowClass);
        action_Load_ini->setObjectName(QStringLiteral("action_Load_ini"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setDocumentMode(true);
        checkTab = new QWidget();
        checkTab->setObjectName(QStringLiteral("checkTab"));
        gridLayout = new QGridLayout(checkTab);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        imageLabel1 = new QLabel(checkTab);
        imageLabel1->setObjectName(QStringLiteral("imageLabel1"));
        imageLabel1->setMinimumSize(QSize(480, 270));
        imageLabel1->setFrameShape(QFrame::StyledPanel);
        imageLabel1->setScaledContents(true);

        verticalLayout->addWidget(imageLabel1);

        imageLabel2 = new QLabel(checkTab);
        imageLabel2->setObjectName(QStringLiteral("imageLabel2"));
        imageLabel2->setMinimumSize(QSize(480, 270));
        imageLabel2->setFrameShape(QFrame::StyledPanel);
        imageLabel2->setScaledContents(true);

        verticalLayout->addWidget(imageLabel2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        logBrowser = new QTextBrowser(checkTab);
        logBrowser->setObjectName(QStringLiteral("logBrowser"));
        logBrowser->setMinimumSize(QSize(400, 0));
        logBrowser->setLineWidth(1);
        logBrowser->setMidLineWidth(0);
        logBrowser->setReadOnly(true);
        logBrowser->setOpenExternalLinks(true);

        gridLayout->addWidget(logBrowser, 0, 1, 1, 1);

        tabWidget->addTab(checkTab, QString());
        realPlayTab = new QWidget();
        realPlayTab->setObjectName(QStringLiteral("realPlayTab"));
        tabWidget->addTab(realPlayTab, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 949, 30));
        menu_Tools = new QMenu(menuBar);
        menu_Tools->setObjectName(QStringLiteral("menu_Tools"));
        menuCamera = new QMenu(menu_Tools);
        menuCamera->setObjectName(QStringLiteral("menuCamera"));
        menu_Settings = new QMenu(menuBar);
        menu_Settings->setObjectName(QStringLiteral("menu_Settings"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menu_Tools->menuAction());
        menuBar->addAction(menu_Settings->menuAction());
        menu_Tools->addAction(menuCamera->menuAction());
        menuCamera->addAction(action_Start);
        menuCamera->addAction(action_Stop);
        menu_Settings->addAction(action_Open_ini);
        menu_Settings->addAction(action_Load_ini);

        retranslateUi(MainWindowClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", Q_NULLPTR));
        action_Start->setText(QApplication::translate("MainWindowClass", "&Start", Q_NULLPTR));
        action_Stop->setText(QApplication::translate("MainWindowClass", "&Stop", Q_NULLPTR));
        action_Open_ini->setText(QApplication::translate("MainWindowClass", "&Open ini", Q_NULLPTR));
        action_Load_ini->setText(QApplication::translate("MainWindowClass", "&Load ini", Q_NULLPTR));
        imageLabel1->setText(QString());
        imageLabel2->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(checkTab), QApplication::translate("MainWindowClass", "Check", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(realPlayTab), QApplication::translate("MainWindowClass", "Real Play", Q_NULLPTR));
        menu_Tools->setTitle(QApplication::translate("MainWindowClass", "&Tools", Q_NULLPTR));
        menuCamera->setTitle(QApplication::translate("MainWindowClass", "&Camera", Q_NULLPTR));
        menu_Settings->setTitle(QApplication::translate("MainWindowClass", "&Settings", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
