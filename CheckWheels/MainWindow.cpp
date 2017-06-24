#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  ui.setupUi(this);
  // isConnectFlag = FALSE;//init not connect
  isPlayFlag = FALSE; // Init not play
  realPlayHandle = (HWND)ui.realPlayTab->winId();

  /*alarm serial port init here*/
  alarmSerial.setPortName("COM3");
  alarmSerial.setBaudRate(QSerialPort::Baud9600);
  if (!alarmSerial.open(QIODevice::ReadWrite)) {
    QMessageBox::warning(
      this, tr("Warning"),
      tr("Can't connect alarm light, error code %2").arg(alarmSerial.error()));
    return;
  }
  alarmData = QByteArray::fromHex("50");
  alarmSerial.write(alarmData);
  // QThread::msleep(500);
  if (alarmSerial.waitForBytesWritten(10)) {
    //! [8] //! [10]
    // read response
    if (alarmSerial.waitForReadyRead(10)) {
      QByteArray responseData = alarmSerial.readAll();
      while (alarmSerial.waitForReadyRead(10))
        responseData += alarmSerial.readAll();

      if (responseData != QByteArray::fromHex("AB")) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Can't connect alarm light, error code %2")
                               .arg(alarmSerial.error()));
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
  // delete ui;
}

void
MainWindow::on_action_Start_triggered()
{
  isPlayFlag = hikvision.hikRealPlay(realPlayHandle);
  qDebug() << "realplay start";
  // image = cv::imread("D:\\2.jpg");
  // cv::imshow("this window",image);
  ////cv::imshow();

  ui.action_Start->setEnabled(!isPlayFlag);
  ui.action_Stop->setEnabled(isPlayFlag);

  while (1) {
    QEventLoop eventloop;
    QTimer::singleShot(400, &eventloop, SLOT(quit())); // wait 0.4s
    eventloop.exec();
    mutex.lock();
    if (isProcessed == true)
      imageNeedProcess.wait(&mutex);
    ui.checkViewer->showImage(pRawImage);
    cv::imshow("fuck the world!", pRawImage);
    mutex.unlock();
    // dosomething process the image
    mutex.lock();
    isProcessed == true;
    imageAllProcessed.wakeAll();
    mutex.unlock();
  }
}

void MainWindow::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();
	QString txt;
	//output the critical message, i.e., when find a wheel stucked! 
	//QString criticalMessage;
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
		break;
	case QtFatalMsg:
		txt = QString("Fatal:");		
		abort();
	}

	//QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//ddd is weekday
	QString current_date = QString("[%1]").arg(current_date_time);
	QString message = QString("%1 %2 %3").arg(current_date).arg(txt).arg(msg);

	ui.logBrowser->append(message);

	QFile outfile("debuglog.txt");
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
  alarmData = QByteArray::fromHex(lightcolor);
  alarmSerial.write(alarmData);
}
