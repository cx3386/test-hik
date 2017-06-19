#include "alarmlight.h"


QT_USE_NAMESPACE

AlarmLight::AlarmLight(QObject *parent)
    : QThread(parent), waitTimeout(10), quit(false)
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
	QSerialPort serial;

	while (!quit) {

		serial.setPortName(portName);
		serial.setBaudRate(9600);
		if (!serial.open(QIODevice::ReadWrite)) {
			emit error(tr("Can't open %1, error code %2")
				.arg(portName).arg(serial.error()));
			qDebug() << tr("Can't open %1, error code %2")
				.arg(portName).arg(serial.error());
			return;
		}

		// write sendData
		QByteArray sendData;
		QByteArray recData;
		QByteArray checkByte(1, 0);
		checkByte[0] = 0xAB;
		switch (currentColor)
		{
		case ALARM_LIGHT_INIT:
			sendData = QString2Hex(tr("50"));
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
				sendData = QString2Hex(tr("51"));
				break;
		case ALARM_LIGHT_RED:
			sendData = QString2Hex(tr("FE"));
			break;
		case ALARM_LIGHT_GREEN:
			sendData = QString2Hex(tr("FD"));
			break;
		case ALARM_LIGHT_YELLOW:
			sendData = QString2Hex(tr("FB"));
			break;
		case ALARM_LIGHT_OFF:
			sendData = QString2Hex(tr("FF"));
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
}

