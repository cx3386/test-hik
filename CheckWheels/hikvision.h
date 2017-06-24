#pragma once
#include <QMutex>
#include <QtDebug>
#include <Windows.h>
#include <opencv.hpp>
//#include <QWidget>
#include <QWaitCondition>

cv::Mat pRawImage;
QMutex mutex;

// In fact no need to block each other, only need block the image process
// do not block the image capture
// QWaitCondition imageAllProcessed;

QWaitCondition imageNeedProcess;
volatile bool isProcessed = false;

class HikVision
{
public:
  BOOL hikRealPlay(HWND handle);
  BOOL hikStopRealPlay();
  LONG lUserID;
  //	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
private:
  LONG lRealPlayHandle;
};
