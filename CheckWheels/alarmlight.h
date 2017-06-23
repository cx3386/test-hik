#ifndef ALARMLIGHT_H
#define ALARMLIGHT_H

/*alarm light color*/
#define ALARM_LIGHT_INIT 0
#define ALARM_LIGHT_RED 1
#define ALARM_LIGHT_GREEN 2
#define ALARM_LIGHT_YELLOW 3
#define ALARM_LIGHT_OFF 4
/*****************************/

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSerialPort>
#include <Windows.h>
#include <QtDebug>


class AlarmLight : public QThread
{
    Q_OBJECT

public:
    explicit AlarmLight(QObject *parent = nullptr);
    ~AlarmLight();
    void run() Q_DECL_OVERRIDE;
	void setAlarmStatus(DWORD dwAlarmColor);

signals:
	void error(const QString &s);

private:
    const QString portName = tr("COM3");
    DWORD dwAlarmColor;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
	QSerialPort serial;

	/*
	//string to hex
	static char ConvertHexChar(char ch)
	{
		if ((ch >= '0') && (ch <= '9'))
			return ch - 0x30;
		else if ((ch >= 'A') && (ch <= 'F'))
			return ch - 'A' + 10;
		else if ((ch >= 'a') && (ch <= 'f'))
			return ch - 'a' + 10;
		else return (-1);
	}

	static QByteArray QString2Hex(QString str)
	{
		QByteArray senddata;
		int hexdata, lowhexdata;
		int hexdatalen = 0;
		int len = str.length();
		senddata.resize(len / 2);
		char lstr, hstr;
		for (int i = 0; i<len; ) {
			hstr = str[i].toLatin1();
			if (hstr == ' ') {
				i++;
				continue;
			}
			i++;
			if (i >= len)
				break;
			lstr = str[i].toLatin1();
			hexdata = ConvertHexChar(hstr);
			lowhexdata = ConvertHexChar(lstr);
			if ((hexdata == 16) || (lowhexdata == 16))
				break;
			else
				hexdata = hexdata * 16 + lowhexdata;
			i++;
			senddata[hexdatalen] = (char)hexdata;
			hexdatalen++;
		}
		senddata.resize(hexdatalen);
		return senddata;
	}
	*/

};

#endif // ALARMLIGHT_H
