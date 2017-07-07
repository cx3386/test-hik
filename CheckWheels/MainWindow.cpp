#include "MainWindow.h"

cv::Mat pRawImage;
QMutex mutex;
//QWaitCondition imageAllProcessed;
QWaitCondition imageNeedProcess;
volatile bool isProcessed = false;

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
	, sensorA(false)
	, sensorB(false)
	, stopSensor(false)
{
  ui.setupUi(this);
  // isConnectFlag = FALSE;//init not connect
  isPlayFlag = FALSE; // Init not play
  realPlayHandle = (HWND)ui.realPlayTab->winId();
  connect(&imageProcess, &CImageProcess::showCheckImage, this, &MainWindow::showCheckImage);
  ui.logBrowser->append("<a href = \"file:///D://1.txt\"> Video");


  imageProcess.start();
  //sendVideoFrame();

  /*alarm serial port init here*/
  //should in the last init list, ensure init won't finish by "return"
  
  alarmSerial.setPortName("COM4");
  alarmSerial.setBaudRate(QSerialPort::Baud9600);
  alarmSerial.setParity(QSerialPort::EvenParity);
  alarmSerial.setDataBits(QSerialPort::Data7);
  alarmSerial.setStopBits(QSerialPort::TwoStop);
  alarmSerial.close();
  if (!alarmSerial.open(QIODevice::ReadWrite)) {
    QMessageBox::warning(
      this, tr("Warning"),
      tr("Can't connect PLC, error code %2").arg(alarmSerial.error()));
  }
  Alarm(ALARM_LIGHT_GREEN);
 
  //startWheelSensor();

  

 }

MainWindow::~MainWindow()
{
  alarmSerial.close();
  // delete ui;
}

void MainWindow::showCheckImage(int num) 
{
	static QVector<QRgb>  sColorTable;
	// only create our color table once
	if (sColorTable.isEmpty())
	{
		for (int i = 0; i < 256; ++i)
			sColorTable.push_back(qRgb(i, i, i));
	}
	mutex.lock();
	QImage image(imageProcess.dstImg[num].data, imageProcess.dstImg[num].cols, imageProcess.dstImg[num].rows, imageProcess.dstImg[num].step, QImage::Format_Indexed8);
	mutex.unlock();
	image.setColorTable(sColorTable);
	QPixmap mp;
	mp.fromImage(image);
	switch (num)
	{
	case 0:
		ui.imageLabel1->setPixmap(mp);
		ui.imageLabel1->show();
		break;
	case 1:
		ui.imageLabel2->setPixmap(mp);
		ui.imageLabel2->show();
	}
}

void MainWindow::on_action_Start_triggered()
{
  isPlayFlag = hikvision.hikRealPlay(realPlayHandle);
  qDebug() << "realplay start";
  // image = cv::imread("D:\\2.jpg");
  // cv::imshow("this window",image);
  ////cv::imshow();

  ui.action_Start->setEnabled(!isPlayFlag);
  ui.action_Stop->setEnabled(isPlayFlag);
  hikvision.hikStartSave();
  //while (1) {
  //  QEventLoop eventloop;
  //  QTimer::singleShot(400, &eventloop, SLOT(quit())); // wait 0.4s
  //  eventloop.exec();
  //  mutex.lock();
  //  if (isProcessed == true)
  //    imageNeedProcess.wait(&mutex);
  //  ui.checkViewer->showImage(pRawImage);
  //  cv::imshow("fuck the world!", pRawImage);
  //  mutex.unlock();
  //  // dosomething process the image
  //  mutex.lock();
  //  isProcessed == true;
  //  imageAllProcessed.wakeAll();
  //  mutex.unlock();
  //}
}

void MainWindow::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();
	QString txt;
	QString uiMessage;
	//output the critical message, i.e., when find a wheel stucked! 

	//QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//ddd is weekday
	QString current_date = QString("[%1]").arg(current_date_time);
	switch (type) {
	case QtDebugMsg:
		txt = QString("Debug:");
		break;
	case QtInfoMsg:
		txt = QString("Info:");
		break;
	case QtWarningMsg:
		txt = QString("Warning:");		
		break;
	case QtCriticalMsg:
		txt = QString("Critical:");		
		//criticalTxt = QString("");
		uiMessage = QString("%1 %2 <a href = \"file:///D://1.txt\">Video").arg(current_date).arg(msg);
		ui.logBrowser->append(uiMessage);
		
		break;
	case QtFatalMsg:
		txt = QString("Fatal:");		
		abort();
	}

	
	QString message = QString("%1 %2 %3").arg(current_date).arg(txt).arg(msg);
	ui.logBrowser->append(message);

	QFile outfile("D:\\log.txt");
	outfile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&outfile);
	text_stream << message << endl;	//"\r\n";
	outfile.flush();
	outfile.close();
	mutex.unlock();
}

void
MainWindow::on_action_Stop_triggered()
{
  if (isPlayFlag) {
    bool flag = hikvision.hikStopRealPlay();

    ui.action_Start->setEnabled(flag);
    ui.action_Stop->setEnabled(!flag);
  }
  isPlayFlag = FALSE;
}

void
MainWindow::Alarm(const char* lightcolor)
{
  alarmData = lightcolor;
  alarmSerial.write(alarmData);
  if (alarmSerial.waitForBytesWritten(100))
  {
	  if (alarmSerial.waitForReadyRead(100))
	  {
		  QByteArray responseData = alarmSerial.readAll();
		  while (alarmSerial.waitForReadyRead(100))
			  responseData += alarmSerial.readAll();
		  if (responseData != "@00WR0045*\r")
			  qCritical() << "Error: Alarm Light wrong return" << endl;

	  }
  }
}

void MainWindow::startWheelSensor()	//0 is error, 1 is ok.
{
	alarmData = "@00RR0000000141*\r";
	while (!stopSensor)
	{
		alarmSerial.write(alarmData);
		if (alarmSerial.waitForBytesWritten(100)) {
			if (alarmSerial.waitForReadyRead(100)) {
				QByteArray responseData = alarmSerial.readAll();
				while (alarmSerial.waitForReadyRead(100))
					responseData += alarmSerial.readAll();

				if (responseData == "@00RR00000040*\r") 
				{
					sensorA = false; sensorB = false;
				}	//00
				else if (responseData == "@00RR00000242*\r") 
				{
					if (sensorA == true && sensorB == false)
						continue;
					else
					{
						sensorA = true; 
						sensorB = false;
						hikvision.hikStartSave();
					}
					
				}	//01
				else if (responseData == "@00RR00000848*\r")
				{
					if (sensorA == true && sensorB == false)
						continue;
					else
					{
						sensorA = false;
						sensorB = true;
						hikvision.hikStopSave();
					}
				}	//10
				else if (responseData == "@00RR00000A31*\r") { qCritical() << "Error: the wheel sensor: A&B" << endl; }	//11
				else qCritical() << "Error: the wheel sensor wrong response" << endl;

			}
			else {
				QString str(tr("Wait read response timeout %1")
					.arg(QTime::currentTime().toString()));
				qCritical() << str << endl;
			}
		}
		else {

			QString str(tr("Wait write request timeout %1")
				.arg(QTime::currentTime().toString()));
			qCritical() << str << endl;
		}
		QEventLoop eventloop;
		QTimer::singleShot(1000, &eventloop, SLOT(quit())); //wait 2s  
		eventloop.exec();
		qDebug() << "SensorA: " << sensorA << "SensorB: " << sensorB << endl;
	}
}
