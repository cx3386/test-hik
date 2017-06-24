#ifndef CIMAGEPROCESS_H
#define CIMAGEPROCESS_H

//#include <QtWidgets>
#include "hikvision.h"
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
//#include <opencv.hpp>

extern cv::Mat pRawImage;
extern QMutex mutex;
extern QWaitCondition imageAllProcessed;
extern QWaitCondition imageNeedProcess;
extern volatile bool isProcessed = false;

using namespace cv;
using namespace std;
class CImageProcess : public QThread
{
  Q_OBJECT

public:
  explicit CImageProcess(QObject* parent = nullptr);
  ~CImageProcess();

  // should have the start control function packaged within the class
  // void transaction(const QString &portName, int waitTimeout, const QString
  // &request);

  void run() Q_DECL_OVERRIDE;

private:
  bool quit;
  volatile int iImgCount;
  int iImgNoCycle;
  // QWaitCondition cond;
};

#endif // CIMAGEPROCESS_H
