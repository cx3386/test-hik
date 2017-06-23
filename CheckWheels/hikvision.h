#pragma once
#include <Windows.h>
#include <opencv2\opencv.hpp>
#include <QMutex>





extern cv::Mat pRawImage;

class  HikVision
{
public:
 
    BOOL hikRealPlay(HWND handle);
    BOOL hikStopRealPlay();
    LONG lUserID;
//	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
private:
	
    LONG lRealPlayHandle;
};
extern QMutex mutex;