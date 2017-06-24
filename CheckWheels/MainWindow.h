#pragma once

#include <QMainWindow>
#include <QtDebug>
#include <QtSerialPort>
#include <QMessageBox>
#include "ui_MainWindow.h"
#include "hikvision.h"
#include "alarm.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
    HWND realPlayHandle;//get opengl widget handle
    BOOL isPlayFlag;

    //BOOL isConnectFlag;
private slots:
    void on_action_Start_triggered();



	void myMessageOutput(QtMsgType type, const QMessageLogContext & context, const QString & msg);

	void on_action_Stop_triggered();

private:
    Ui::MainWindowClass ui;
    //cv::Mat image;
	QSerialPort alarmSerial;
	QByteArray alarmData;
    HikVision hikvision;
	void Alarm(const char* lightcolor);
	//void loopShow();
};
