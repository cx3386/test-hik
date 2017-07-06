#ifndef CIMAGEPROCESS_H
#define CIMAGEPROCESS_H

//#include <QtWidgets>
#include "hikvision.h"
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
//#include <opencv.hpp>
//cv::Mat pRawImage;
//QMutex mutex;
////extern QWaitCondition imageAllProcessed;
//QWaitCondition imageNeedProcess;
//volatile bool isProcessed = false;

using namespace cv;
using namespace std;
class CImageProcess : public QThread
{
  Q_OBJECT

public:
  explicit CImageProcess(QObject* parent = nullptr);
  ~CImageProcess();
  Mat dstImg[2];

  // should have the start control function packaged within the class
  // void transaction(const QString &portName, int waitTimeout, const QString
  // &request);

  void run() Q_DECL_OVERRIDE;
signals:
  void showCheckImage(int num);
  
private:
  bool quit;
  volatile int iImgCount;
  int iImgNoCycle;
  bool isSameWheel;
  double angleSum;
  int angleCount;
  double avgAngle;
  void oneWheel();
  double angleLowThreshold, angleHighThreshold;
  // QWaitCondition cond;
};

#endif // CIMAGEPROCESS_H
