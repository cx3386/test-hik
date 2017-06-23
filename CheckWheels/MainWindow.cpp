#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //isConnectFlag = FALSE;//初始未未连接
    isPlayFlag = FALSE;//初始时为没有播放
    realPlayHandle = (HWND)ui.realPlayTab->winId();

	/*alarm serial port init here*/
	alarmSerial.setPortName("COM3");
	alarmSerial.setBaudRate(QSerialPort::Baud9600);
	if (!alarmSerial.open(QIODevice::ReadWrite)) {
		QMessageBox::warning(this, tr("Warning"), tr("Can't connect alarm light, error code %2").arg(alarmSerial.error()));
		return;
	}
	alarmData = QByteArray::fromHex("50");
	alarmSerial.write(alarmData);
	//QThread::msleep(500);
	if (alarmSerial.waitForBytesWritten(10)) {
		//! [8] //! [10]
		// read response
		if (alarmSerial.waitForReadyRead(10)) {
			QByteArray responseData = alarmSerial.readAll();
			while (alarmSerial.waitForReadyRead(10))
				responseData += alarmSerial.readAll();

			if (responseData != QByteArray::fromHex("AB")) 
			{ 
				QMessageBox::warning(this, tr("Warning"), tr("Can't connect alarm light, error code %2").arg(alarmSerial.error()));
				return; 
			}
		}
	}
	alarmData = QByteArray::fromHex("51");
	alarmSerial.write(alarmData);
}

MainWindow::~MainWindow()
{
	alarmSerial.close();
	//delete ui;
}

void MainWindow::on_action_Start_triggered()
{
    isPlayFlag = hikvision.hikRealPlay(realPlayHandle);
    qDebug() << "realplay start";
    //image = cv::imread("D:\\2.jpg");
    //cv::imshow("this window",image);
    ////cv::imshow();
	
    
    ui.action_Start->setEnabled(!isPlayFlag);
    ui.action_Stop->setEnabled(isPlayFlag);
	
	//while(1)
	//{
	//	QEventLoop eventloop;
	//	QTimer::singleShot(400, &eventloop, SLOT(quit())); //wait 0.4s
	//	eventloop.exec();
	//	mutex.lock();
	//	ui.checkViewer->showImage(pRawImage);
	//	imshow("fuck the world!", pRawImage);
	//	mutex.unlock();
	//}
}

void MainWindow::on_action_Stop_triggered()
{
    if(isPlayFlag) {
        bool flag = hikvision.hikStopRealPlay();

        ui.action_Start->setEnabled(flag);
        ui.action_Stop->setEnabled(!flag);
    }
    isPlayFlag = FALSE;
	
}

void MainWindow::Alarm(const char * lightcolor)
{
	alarmData = QByteArray::fromHex(lightcolor);
	alarmSerial.write(alarmData);
}
