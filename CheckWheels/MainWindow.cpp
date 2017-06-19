#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //isConnectFlag = FALSE;//初始未未连接
    isPlayFlag = FALSE;//初始时为没有播放
    realPlayHandle = (HWND)ui.realPlayTab->winId();
	alarm.setAlarmStatus(ALARM_LIGHT_INIT);
}


void MainWindow::on_action_Start_triggered()
{
    isPlayFlag = hikvision.hikRealPlay(realPlayHandle);
    qDebug() << "realplay start";
    //image = cv::imread("D:\\2.jpg");
    //cv::imshow("this window",image);
    ////cv::imshow();
    //ui.checkViewer->showImage(image);
    ui.action_Start->setEnabled(!isPlayFlag);
    ui.action_Stop->setEnabled(isPlayFlag);
	alarm.setAlarmStatus(ALARM_LIGHT_GREEN);
}


void MainWindow::on_action_Stop_triggered()
{
    if(isPlayFlag) {
        bool flag = hikvision.hikStopRealPlay();

        ui.action_Start->setEnabled(flag);
        ui.action_Stop->setEnabled(!flag);
    }
    isPlayFlag = FALSE;
	alarm.setAlarmStatus(ALARM_LIGHT_RED);
}
