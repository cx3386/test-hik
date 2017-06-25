#include "hikvision.h"
#include <iostream>

using namespace std;
using namespace cv;

extern cv::Mat pRawImage;
extern QMutex mutex;
//extern QWaitCondition imageAllProcessed;
extern QWaitCondition imageNeedProcess;
extern volatile bool isProcessed;

int sendVideoFrame()
{
	//����Ƶ�ļ�����ʵ���ǽ���һ��VideoCapture�ṹ????
	VideoCapture capture("C:\\Users\\cx3386\\Videos\\1.mp4");
	//����Ƿ�������:�ɹ���ʱ��isOpened����ture????
	if (!capture.isOpened())
		cout << "fail??to??open!" << endl;
	//��ȡ����֡��????
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;

	//qDebug() << "format:" << capture.get(CV_CAP_PROP_FORMAT) << endl;

	//���ÿ�ʼ֡()????
	long frameToStart = 100;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	cout << "�ӵ�" << frameToStart << "֡��ʼ��" << endl;


	//���ý���֡????
	int frameToStop = 10000;

	if (frameToStop < frameToStart)
	{
		cout << "����֡С�ڿ�ʼ֡��������󣬼����˳���" << endl;
		return -1;
	}

	else
	{
		cout << "����֡Ϊ����" << frameToStop << "֡" << endl;
	}


	//��ȡ֡��????
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "֡��Ϊ:" << rate << endl;



	//����һ���������ƶ�ȡ��Ƶѭ�������ı���????
	bool stop = false;
	//����ÿһ֡��ͼ��????
	Mat frame;
	//��ʾÿһ֡�Ĵ���????
	namedWindow("Extracted frame");
	//��֡��ļ��ʱ��:????
	//int??delay??=??1000/rate;????
	int delay = 1000 / rate;


	//����whileѭ����ȡ֡????
	//currentFrame����ѭ�����п��ƶ�ȡ��ָ����֡��ѭ�������ı���????
	long currentFrame = frameToStart;


	//�˲����ĺ�????
	//int kernel_size = 3;
	//Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);
	volatile int iImgHanding = 3;
	while (!stop)
	{
		//��ȡ��һ֡????
		if (!capture.read(frame))
		{
			cout << "��ȡ��Ƶʧ��" << endl;
			return - 1;
		}

		//������˲�����????
		//imshow("Extracted??frame", frame);
		//filter2D(frame, frame, -1, kernel);

		//imshow("after??filter", frame);
		cout << "���ڶ�ȡ��" << currentFrame << "֡" << endl;
		//waitKey(int??delay=0)��delay??��??0ʱ����Զ�ȴ�����delay>0ʱ��ȴ�delay����????
		//��ʱ�����ǰû�а�������ʱ������ֵΪ-1�����򷵻ذ���????

		int c = cv::waitKey(delay);
		//����ESC���ߵ���ָ���Ľ���֡���˳���ȡ��Ƶ????
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		//���°������ͣ���ڵ�ǰ֡���ȴ���һ�ΰ���????
		//if (c >= 0)
		//{
		//	waitKey(0);
		//}
		currentFrame++;
		if (iImgHanding) {
			iImgHanding--;
			continue;
		}

		cvtColor(frame, frame, CV_BGR2GRAY);
		imshow("Extracted frame", frame);
		mutex.lock();
		pRawImage = frame;
		imageNeedProcess.wakeOne();
		mutex.unlock();
		iImgHanding = 3;
	}
	//�ر���Ƶ�ļ�????
	capture.release();
	cv::waitKey(0);
	return 0;
}