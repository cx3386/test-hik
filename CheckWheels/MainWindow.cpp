#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	lUserID = -1;
	//isConnectFlag = FALSE;//初始未未连接
	isPlayFlag = FALSE;//初始时为没有播放
}

HWND MainWindow::getRealPlayHandle()
{
    return (HWND)ui.realPlayTab->winId();
}

void MainWindow::on_action_Start_triggered()
{
    isPlayFlag = hikRealPlay(getRealPlayHandle());
    qDebug() << "realplay finished";
    //image = cv::imread("D:\\2.jpg");
    //cv::imshow("this window",image);
    ////cv::imshow();
    //ui.checkViewer->showImage(image);
	ui.action_Start->setEnabled(!isPlayFlag);
	ui.action_Stop->setEnabled(isPlayFlag);
}


void MainWindow::on_action_Stop_triggered()
{
	if(isPlayFlag)
	{
		bool flag = hikStopRealPlay();
	
		ui.action_Start->setEnabled(flag);
		ui.action_Stop->setEnabled(!flag); 
	}
	isPlayFlag = FALSE;
}
