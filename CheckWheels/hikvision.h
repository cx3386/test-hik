#pragma once
#include <QMutex>
#include <QtDebug>
#include <Windows.h>
#include <opencv2/opencv.hpp>
//#include <QWidget>
#include <QWaitCondition>

extern cv::Mat pRawImage;
extern QMutex mutex;
int sendVideoFrame();

// In fact no need to block each other, only need block the image process
// do not block the image capture
// QWaitCondition imageAllProcessed;

extern QWaitCondition imageNeedProcess;
extern volatile bool isProcessed;

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
