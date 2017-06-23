#include "alarmlight.h"


QT_USE_NAMESPACE

AlarmLight::AlarmLight(QObject *parent)
	: QThread(parent), waitTimeout(5), quit(false)
{
}

AlarmLight::~AlarmLight()
{
	mutex.lock();
	quit = true;
	cond.wakeOne();
	mutex.unlock();
	wait();
}
void AlarmLight::setAlarmStatus(DWORD dwAlarmColor)
{
	QMutexLocker locker(&mutex);
	this->dwAlarmColor = dwAlarmColor;
	if (!isRunning())
		start();
	else
		cond.wakeOne();
}

void AlarmLight::run()
{
	mutex.lock();
	DWORD currentColor = dwAlarmColor;
	mutex.unlock();
	//QSerialPort serial;

	while (!quit) {
		//

		// write sendData
		QByteArray sendData;
		QByteArray recData;
		//QByteArray checkByte(1, 0);
		//checkByte[0] = 0xAB;
		QByteArray checkByte = QByteArray::fromHex("AB");
		switch (currentColor)
		{
		case ALARM_LIGHT_INIT:

			//serial.close();
			serial.setPortName(portName);
			serial.setBaudRate(9600);
			if (!serial.open(QIODevice::ReadWrite)) {
				emit error(tr("Can't open %1, error code %2")
					.arg(portName).arg(serial.error()));
				qDebug() << tr("Can't open %1, error code %2")
					.arg(portName).arg(serial.error());
				return;
			}

			sendData = QByteArray::fromHex("50");
			serial.write(sendData);
			serial.waitForBytesWritten(waitTimeout);
			if (serial.waitForReadyRead(waitTimeout)) {
				recData = serial.readAll();
				while (serial.waitForReadyRead(10))
					recData += serial.readAll();
				if (recData != checkByte)
				{
					emit error(tr("Can't open %1, relay no response!")
						.arg(portName));
					qDebug() << tr("Can't open %1, relay no response!").arg(portName);
					return;
				}
			sendData = QByteArray::fromHex("51");
			QThread::msleep(1000);
			break;
		case ALARM_LIGHT_RED:
			sendData = QByteArray::fromHex("FE");
			break;
		case ALARM_LIGHT_GREEN:
			sendData = QByteArray::fromHex("FD");
			break;
		case ALARM_LIGHT_YELLOW:
			sendData = QByteArray::fromHex("FB");
			break;
		case ALARM_LIGHT_OFF:
			sendData = QByteArray::fromHex("FF");
			break;
		};

		serial.write(sendData);
		serial.waitForBytesWritten(waitTimeout);

		mutex.lock();
		cond.wait(&mutex);
		currentColor = dwAlarmColor;
		mutex.unlock();
	}
}


