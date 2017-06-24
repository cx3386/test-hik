#include "cimageprocess.h"

CImageProcess::CImageProcess(QObject* parent)
	: QThread(parent)
	, quit(false)
	, iImgCount(0)
	, iImgNoCycle(0)//, waitTimeout(0)
{

}

CImageProcess::~CImageProcess()
{
	mutex.lock();
	quit = true;
	// cond.wakeOne();
	mutex.unlock();
	wait(); // wait and exit until the thread finishes its work
}

void CImageProcess::run()
{
	//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------
	//��������
	int gs1 = 7;//����Բ���ʱ��˹�˲��ں˴�С��ֻ��Ϊ������Խ��Խģ����Բ�뱳�����ֲ�����ʱ����С��˹ֵ
	double dp = 1.5;//Բ���ۼ���ͼ��ֱ���������ͼ��֮�ȵĵ�����ֵԽС�������ۼ����ֱ���Խ�ߣ�ʶ��Բ�������
	double minDist = 20;//����任��⵽��Բ��Բ��֮�����С���룬ֵԽ��ĳЩԲ���ⲻ������ֵԽС��������ڵ�Բ�ᱻ����ļ���һ���غϵ�Բ
	double param1 = 200;//���ݸ�canny��Ե������ӵĸ���ֵ���ҵ���ֵΪ����ֵ��һ��
	double param2 = 100;//���׶�Բ���ۼ�������ֵ��ֵԽ�󣬼�⵽��ԲԽ������ֵԽС�����Լ�⵽������������ڵ�Բ
	double s = 3;//������ƥ���������С����ı�,���һֱ����ʵ8�����ʵ������ֵ����Сgs2
	int gs2 = 9;//����ƥ��ʱ��˹�˲��ں˴�С��ֻ��Ϊ������Խ��Խģ���������˹ֵ����ʹ����ƥ��ʱ��������Ҫ������

	Mat dstImg;

	while (!quit) {
		// wait until a image need process
		mutex.lock();
		imageNeedProcess.wait(&mutex);
		dstImg = pRawImage;
		mutex.unlock();

		//********************����Բ��Ⲣ�ָ�Բ��**************************//
		cv::GaussianBlur(dstImg, dstImg, Size(gs1, gs1), 2, 2);//�޸��ں˴�С�ɸ���ʶ��Բ�����ף�ԽСԽ����
		vector<Vec3f> circles;
		//����Բ
		HoughCircles(dstImg, circles, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, 0, 0); //���޸Ĳ������ڲ�׽Բ��׼ȷ��
		if (circles.size() < 1) {
			qDebug("��%d��δʶ��Բ\n",++iImgNoCycle);
			continue; 
		}
		//���е�һ��Բͼ��ָ�
		Point center0(cvRound(circles[0][0]),
			cvRound(circles[0][1])); //����ڶ���Բ�ָ�뾶��Բ��
		int radius0 = cvRound(circles[0][2]) - 10;
		//����Բ��
		// circle(imagetemp, center0, 3, Scalar(0, 255, 0), -1, 8, 0);
		//����Բ����
		// circle(imagetemp, center0, radius0, Scalar(155, 50, 255), 3, 8, 0);
		imshow("��׽Ч��ͼ", imagetemp0);
		
		qDebug("5");
		Mat imagetemp1(imagetemp0.rows, imagetemp0.cols,
			imagetemp0.type()); //�����һ���и��ͼ�����
		for (int x = 0; x < imagetemp0.cols; x++) {
			for (int y = 0; y < imagetemp0.rows; y++) {
				int temp = ((x - center0.x) * (x - center0.x) +
					(y - center0.y) * (y - center0.y));
				if (temp < (radius0 * radius0)) {
					imagetemp1.at<Vec3b>(Point(x, y))[0] =
						imagetemp0.at<Vec3b>(Point(x, y))[0];
					imagetemp1.at<Vec3b>(Point(x, y))[1] =
						imagetemp0.at<Vec3b>(Point(x, y))[1];
					imagetemp1.at<Vec3b>(Point(x, y))[2] =
						imagetemp0.at<Vec3b>(Point(x, y))[2];
				}
			}
		}
		//���еڶ���Բͼ��ָ�
		int radius1 = radius0 / 2 - 10; //����ڶ���Բ�ָ�뾶�뾶
		Mat imagetemp2(imagetemp1.rows, imagetemp1.cols,
			imagetemp1.type()); //����ָ��ͼ��
		for (int x = 0; x < imagetemp1.cols; x++) {
			for (int y = 0; y < imagetemp1.rows; y++) {
				int temp = ((x - center0.x) * (x - center0.x) +
					(y - center0.y) * (y - center0.y));
				if (temp > (radius1 * radius1)) {
					imagetemp2.at<Vec3b>(Point(x, y))[0] =
						imagetemp1.at<Vec3b>(Point(x, y))[0];
					imagetemp2.at<Vec3b>(Point(x, y))[1] =
						imagetemp1.at<Vec3b>(Point(x, y))[1];
					imagetemp2.at<Vec3b>(Point(x, y))[2] =
						imagetemp1.at<Vec3b>(Point(x, y))[2];
				}
			}
		}
		//���ָ��ͼ���С����ΪԲ����С
		Mat imagetemp3(2 * radius0, 2 * radius0,
			imagetemp0.type()); //���������ͼ��
		for (int x = 0; x < imagetemp2.cols; x++) {
			for (int y = 0; y < imagetemp2.rows; y++) {
				int temp = ((x - center0.x) * (x - center0.x) +
					(y - center0.y) * (y - center0.y));
				int tempx = x - center0.x + radius0; //����ͼ���������任
				int tempy = y - center0.y + radius0;
				if (temp < (radius0 * radius0)) {
					imagetemp3.at<Vec3b>(Point(tempx, tempy))[0] =
						imagetemp2.at<Vec3b>(Point(x, y))[0];
					imagetemp3.at<Vec3b>(Point(tempx, tempy))[1] =
						imagetemp2.at<Vec3b>(Point(x, y))[1];
					imagetemp3.at<Vec3b>(Point(tempx, tempy))[2] =
						imagetemp2.at<Vec3b>(Point(x, y))[2];
				}
			}
		}
		outimage[i] = imagetemp3;
		qDebug("6");
		i++;
		if (i == 2)
			break;
	}
}

//�����εõ���Բ��ͼ�����Ϊͬ����С
Mat imagetemp4;
resize(outimage[1], imagetemp4, Size(outimage[0].cols, outimage[0].rows), 0,
	0, CV_INTER_LINEAR);
outimage[1] = imagetemp4;

//**************************ͼ������ƥ�䲿��*****************************************//

cvtColor(outimage[0], outimage[0],
	CV_BGR2GRAY); //���и���ͼƬת��Ϊ�Ҷ�ͼ
cvtColor(outimage[1], outimage[1], CV_BGR2GRAY);
GaussianBlur(outimage[0], outimage[0], Size(7, 7), 2,
	2); //�޸��ں˴�С�ɸ���ʶ��Բ�����ף�ԽСԽ����
GaussianBlur(outimage[1], outimage[1], Size(7, 7), 2, 2);
// imshow("��һ��ͼ�и�Ч��ͼ", outimage[0]);
// imshow("�ڶ���ͼ�и�Ч��ͼ", outimage[1]);

//ʹ��SURF���Ӽ��ؼ���
int minHessian = 400; // SURF�㷨�е�hessian��ֵ
SurfFeatureDetector detector(
	minHessian); //����һ��SurfFeatureDetector��SURF�� ������������
vector<KeyPoint> keypoints_object,
keypoints_scene; // vectorģ���࣬����������͵Ķ�̬����

//����detect��������SURF�����ؼ��㣬������vector������
detector.detect(outimage[0], keypoints_object);
detector.detect(outimage[1], keypoints_scene);

//����������������������
SurfDescriptorExtractor extractor;
Mat descriptors_object, descriptors_scene;
extractor.compute(outimage[0], keypoints_object, descriptors_object);
extractor.compute(outimage[1], keypoints_scene, descriptors_scene);

//ʹ��FLANNƥ�����ӽ���ƥ��
FlannBasedMatcher matcher;
vector<DMatch> matches;
matcher.match(descriptors_object, descriptors_scene, matches);
double max_dist = 0;
double min_dist = 100; //��С�����������

//������ؼ���֮���������ֵ����Сֵ
for (int i = 0; i < descriptors_object.rows; i++) {
	double dist = matches[i].distance;
	if (dist < min_dist)
		min_dist = dist;
	if (dist > max_dist)
		max_dist = dist;
}

//����ƥ�����С��1.5*min_dist�ĵ��
std::vector<DMatch> good_matches;
for (int i = 0; i < descriptors_object.rows; i++) {
	if (matches[i].distance <
		s * min_dist) // 1 * min_dist<*******************************&
	// descriptors_object.rows*descriptors_object.cols>(radius1+5)*(radius1+5)
	{
		good_matches.push_back(matches[i]);
	}
}
qDebug("7");

//�ж���Чƥ�����Ƿ���ڵ���3��
if (good_matches.size() < 3)
	continue;
qDebug("8");

//���Ƴ�ƥ�䵽�Ĺؼ���
Mat image_matches;
drawMatches(outimage[0], keypoints_object, outimage[1], keypoints_scene,
	good_matches, image_matches, Scalar::all(-1), Scalar::all(-1),
	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

//��ʾ���ս��
imshow("������ƥ��ͼ", image_matches);
qDebug("9");

//***********************�������任���󣬼�����ת�Ƕ�**************************//
//���������ֲ�����
vector<Point2f> obj;
vector<Point2f> scene;

//��ƥ��ɹ���ƥ����л�ȡ�ؼ���
for (unsigned int i = 0; i < good_matches.size(); i++) {
	obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
	scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
}

//�������ŷ���任����
Mat M = estimateRigidTransform(obj, scene, false);
if (M.data == NULL)
continue;
qDebug("f");
// cout << "M (default) = " << endl << M << endl <<
// endl;//������ŷ���任����

//������ת�Ƕ�
double* data = M.ptr<double>(0); //���������ַ
double t = data[1];              //�����1�е�2����
double f = -asin(t) / 3.1415926 * 180;
qDebug("����˳ʱ��ת��%lf��\n", f);
	}
}
}
