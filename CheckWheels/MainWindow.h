#pragma once

#include <QtWidgets/QMainWindow>
#include <QtDebug>
#include "ui_MainWindow.h"
#include "hikvision.h"

class MainWindow : public QMainWindow, public HikVision
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	HWND getRealPlayHandle();//ȡ�ô�����һ��GLWIdget�ľ��

private slots:
    void on_action_Start_triggered();

private:
	Ui::MainWindowClass ui;
};
