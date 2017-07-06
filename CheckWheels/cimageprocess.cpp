#include "cimageprocess.h"
#include "qmath.h"
#include <opencv2/features2d.hpp>
#include <QTime>
//#include <nonfree\nonfree.hpp>

//using namespace cv::xfeatures2d;

CImageProcess::CImageProcess(QObject* parent)
	: QThread(parent)
	, quit(false)
	, iImgCount(0)
	, iImgNoCycle(0)
	, isSameWheel(false)
	, angleCount(0)
	, angleSum(0)//, waitTimeout(0)
	, angleLowThreshold(2.0)
	, angleHighThreshold(3.0)
{

}

CImageProcess::~CImageProcess()
{
	mutex.lock();
	quit = true;
	imageNeedProcess.wakeOne();
	mutex.unlock();
	wait(); // wait and exit until the thread finishes its work
}

void CImageProcess::oneWheel()
{
	if (isSameWheel)
	{
		//calculate the avg angle
		avgAngle = angleSum / angleCount;
		if (avgAngle < angleLowThreshold || avgAngle > angleHighThreshold)
		{
			qCritical("Error: wheel too Slow");
		}
		qCritical("angle: %lf Degree; angleCount: %d \n", avgAngle, angleCount);
		angleSum = 0;
		angleCount = 0;
		isSameWheel = false;
	}
}

void CImageProcess::run()
{
	//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------
	//��������
	int gs1 = 7;//Խ��Խ�죬׼ȷ��Խ��
	//����Բ���ʱ��˹�˲��ں˴�С��ֻ��Ϊ������Խ��Խģ����Բ�뱳�����ֲ�����ʱ����С��˹ֵ
	double dp = 1;//Բ���ۼ���ͼ��ֱ���������ͼ��֮�ȵĵ�����ֵԽС�������ۼ����ֱ���Խ�ߣ�ʶ��Բ�������
	double minDist = 90;//����任��⵽��Բ��Բ��֮�����С���룬ֵԽ��ĳЩԲ���ⲻ������ֵԽС��������ڵ�Բ�ᱻ����ļ���һ���غϵ�Բ
	double param1 = 300;//���ݸ�canny��Ե������ӵĸ���ֵ���ҵ���ֵΪ����ֵ��һ��
	double param2 = 60;//50~80����ʶ�𲻵���С���ҵ��ܶ�Բ��Խ��Խ��
	//���׶�Բ���ۼ�������ֵ��ֵԽ�󣬼�⵽��ԲԽ������ֵԽС�����Լ�⵽������������ڵ�Բ
	//double s = 1.5;//������ƥ���������С����ı�,���һֱ����ʵ8�����ʵ������ֵ����Сgs2
	int gs2 = 9;//����ƥ��ʱ��˹�˲��ں˴�С��ֻ��Ϊ������Խ��Խģ���������˹ֵ����ʹ����ƥ��ʱ��������Ҫ������

	 Mat srcImg;
	

	while (!quit) {
		// wait until a image need process
		mutex.lock();
		imageNeedProcess.wait(&mutex);
		if (quit == true)
		{
			mutex.unlock();
			break;
		}
		srcImg = pRawImage;
		mutex.unlock();

		QTime time;
		time.start();
		resize(srcImg, srcImg, Size(1280, 720), 0, 0, CV_INTER_LINEAR);
		
		Rect rect(200, 0, 880, 720);
		Mat roiImage = srcImg(rect);
		Mat blurImage;//use blurimage to houghcircles
		
		//srcImg(rect).copyTo(roiImage);
		//********************����Բ��Ⲣ�ָ�Բ��**************************//
		GaussianBlur(roiImage, blurImage, Size(gs1, gs1), 2, 2);//�޸��ں˴�С�ɸ���ʶ��Բ�����ף�ԽСԽ����
		vector<Vec3f> circles;
		//����Բ
		HoughCircles(blurImage, circles, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, 200, 300); //���޸Ĳ������ڲ�׽Բ��׼ȷ��
		//if (circles.size() < minCycleSize || circles.size() > maxCycleSize) //is it diameter or radium?
		if (circles.size() < 1)
		{
		//	qDebug("No cycle:%d\n", ++iImgNoCycle);
			iImgCount = 0;
			oneWheel();
			continue;
		}
		/*judge if the circle is out of ROI*/
		Point center0(cvRound(circles[0][0]), cvRound(circles[0][1])); //Բ������
		int radiusOutside = cvRound(circles[0][2]) - 10;	//Բ���⾶
		int radiusInside = radiusOutside / 1.4; //Բ���ھ�
		
		//if the circle is out of ROI, continue
		if ((center0.x + radiusOutside) > roiImage.cols - 1||
			(center0.x - radiusOutside) < 0 ||
			(center0.y + radiusOutside) > roiImage.rows - 1||
			(center0.y - radiusOutside) < 0)
		{
		//	qDebug("Cycle out:%d\n", ++iImgNoCycle);
			iImgCount = 0;
			oneWheel();
			continue;
		}
		iImgCount++;
		
		//qDebug() << circles.size << endl;
		//qDebug() << "circles[0][0]" << circles[0][0];
		//qDebug() << "Radius:" << circles[0][2] << endl;

		Rect ringRect((center0.x - radiusOutside), (center0.y - radiusOutside),
			(2 * radiusOutside + 1), (2 * radiusOutside + 1));
		Mat imageRing = roiImage(ringRect);
		Mat mask = Mat::zeros(imageRing.size(), CV_8UC1);
		circle(mask, Point(mask.rows / 2, mask.cols / 2), radiusOutside, Scalar(255), -1, 8);
		bitwise_not(mask, mask);
		circle(mask, Point(mask.rows / 2, mask.cols / 2), radiusInside, Scalar(255), -1, 8);
		imageRing.setTo(0, mask);
		//imshow("mask", mask);
		//imshow("imageRing1", imageRing);

	
		/*********************������Ч�ʷǳ��ͣ�80ms��*********************/
		/*Mat imageRing(2 * radiusOutside + 1, 2 * radiusOutside + 1, roiImage.type()); //����Բ��,����Ȧ�ӿ�

		for (int ringX = 0; ringX < imageRing.cols; ringX++) {
			for (int ringY = 0; ringY < imageRing.rows; ringY++) {
				int roiX = ringX + center0.x - radiusOutside ; //����ͼ���������任
				int roiY = ringY + center0.y - radiusOutside ;
				int temp = ((roiX - center0.x) * (roiX - center0.x) +
					(roiY - center0.y) * (roiY - center0.y));

				if (temp >(radiusInside * radiusInside) && temp < (radiusOutside * radiusOutside))
				{
					imageRing.at<uchar>(Point(ringX, ringY)) =
						roiImage.at<uchar>(Point(roiX, roiY));
				}
			}
		}*/
		
		if (iImgCount == 1)
		{
			dstImg[0] = imageRing;
			emit showCheckImage(0);//ui->show
			continue;
		}
		if (iImgCount == 2)
		{
			dstImg[1] = imageRing;
			emit showCheckImage(1);
			iImgCount = 0;//drop the all image//discrete sampling
		}
		//�����εõ���Բ��ͼ�����Ϊͬ����С
		//resize the first dstImg same as the second
		//so that radius is right
		
		resize(dstImg[0], dstImg[0], Size(dstImg[1].cols, dstImg[1].rows), 0, 0, CV_INTER_LINEAR);

		//imshow("Ring", dstImg[1]);

		//**************************ͼ������ƥ�䲿��*****************************************//

		//cvtColor(dstImg[0], dstImg[0], CV_BGR2GRAY); //���и���ͼƬת��Ϊ�Ҷ�ͼ
		//cvtColor(dstImg[1], dstImg[1], CV_BGR2GRAY);
		//GaussianBlur(dstImg[0], dstImg[0], Size(7, 7), 2, 2); //�޸��ں˴�С�ɸ���ʶ��Բ�����ף�ԽСԽ����
		//GaussianBlur(dstImg[1], dstImg[1], Size(7, 7), 2, 2);
		
		/******************ORB�㷨*********************/
		Ptr<ORB> orb = ORB::create(600);
		vector<KeyPoint> allkeypoints1, allkeypoints2;
		orb->detect(dstImg[0], allkeypoints1);
		orb->detect(dstImg[1], allkeypoints2);
		vector<KeyPoint> keypoints1, keypoints2; 
		
		for (int i = 0; i < allkeypoints1.size(); i++)
		{
			int temp = (allkeypoints1[i].pt.x - dstImg[0].cols / 2) * (allkeypoints1[i].pt.x - dstImg[0].cols / 2)
				+ (allkeypoints1[i].pt.y - dstImg[0].rows / 2) * (allkeypoints1[i].pt.y - dstImg[0].rows / 2);
			if (temp > ((radiusInside + 10) * (radiusInside + 10)) && temp < ((radiusOutside - 10) * (radiusOutside - 10)))
			{
				keypoints1.push_back(allkeypoints1[i]);
				circle(dstImg[0], allkeypoints1[i].pt, 2, Scalar(255, 255, 255));
			}
		}
		for (int i = 0; i < allkeypoints2.size(); i++)
		{
			int temp = (allkeypoints2[i].pt.x - dstImg[1].cols / 2) * (allkeypoints2[i].pt.x - dstImg[1].cols / 2)
				+ (allkeypoints2[i].pt.y - dstImg[1].rows / 2) * (allkeypoints2[i].pt.y - dstImg[1].rows / 2);
			if (temp > ((radiusInside + 10) * (radiusInside + 10)) && temp < ((radiusOutside - 10) * (radiusOutside - 10)))
			{
				keypoints2.push_back(allkeypoints2[i]);
				circle(dstImg[1], allkeypoints2[i].pt, 2, Scalar(255, 255, 255));
			}
		}
		//imshow("dstImg[0]", dstImg[0]);
		//imshow("dstImg[1]", dstImg[1]);


		Mat descriptors1, descriptors2;
		orb->compute(dstImg[0], keypoints1, descriptors1);
		orb->compute(dstImg[1], keypoints2, descriptors2);
		
		//����ƥ��
		Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
		
		//Lowe's algorithm
		//�����㷨ѡ�������ƥ��
		vector< vector<DMatch> > matches;
		matcher->knnMatch(descriptors1, descriptors2, matches, 2);
		vector<DMatch> good_matches;
		//������ؼ���֮���������ֵ����Сֵ
		/*
		double max_dist = 0;
		double min_dist = 50;
		for (int i = 0; i < matches.size(); i++) {
			double dist = matches[i][0].distance;
			if (dist < min_dist)
				min_dist = dist;
			if (dist > max_dist)
				max_dist = dist;
		}
		*/
		//qDebug() << "min_dist" << min_dist << endl;
		for (unsigned i = 0; i < matches.size(); i++) {
			if (matches[i][0].distance < 0.6 * matches[i][1].distance//0.6
				) {
				//&&matches[i][0].distance < 3 * min_dist
				good_matches.push_back(matches[i][0]);
			}
		}
		
			/*
		//ʹ��SURF���Ӽ��ؼ���
		int minHessian = 400; // SURF�㷨�е�hessian��ֵ
		Ptr<SURF> detector = SURF::create(minHessian);//����һ��SurfFeatureDetector��SURF�� ������������
		//SurfFeatureDetector detector(minHessian);
		vector<KeyPoint> keypoints_object, keypoints_scene; // vectorģ���࣬����������͵Ķ�̬����

		//����detect��������SURF�����ؼ��㣬������vector������
		detector->detect(dstImg[0], keypoints_object);
		detector->detect(dstImg[1], keypoints_scene);

		//����������������������
		//SurfDescriptorExtractor extractor;
		Mat descriptors_object, descriptors_scene;
		detector->compute(dstImg[0], keypoints_object, descriptors_object);
		detector->compute(dstImg[1], keypoints_scene, descriptors_scene);

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
		

		//����ƥ�����С��1.5*min_dist�ĵ�� //Խ��Խ��ȷ
		vector<DMatch> good_matches;
		for (int i = 0; i < descriptors1.rows; i++) {
			if (matches[i].distance <
				1.5  * min_dist) // 1 * min_dist<*******************************&
			// descriptors_object.rows*descriptors_object.cols>(radius1+5)*(radius1+5)
			{
				good_matches.push_back(matches[i]);
			} 
		}
		*/
		//qDebug("7");

		//�ж���Чƥ�����Ƿ���ڵ���10��
		if (good_matches.size() < 3)
		{
			//qDebug("the match point less than 3");
			continue;
		}
		

		//���Ƴ�ƥ�䵽�Ĺؼ���
		Mat image_matches;
		drawMatches(dstImg[0], keypoints1, dstImg[1], keypoints2,
			good_matches, image_matches, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		//��ʾ���ս��
 		imshow("������ƥ��ͼ", image_matches);
		//qDebug() << "match" << time.elapsed() / 1000.0 << "s" << endl;
		waitKey(30);

		

		//***********************�������任���󣬼�����ת�Ƕ�**************************//
		//���������ֲ�����
		vector<Point2f> obj;
		vector<Point2f> scene;

		//��ƥ��ɹ���ƥ����л�ȡ�ؼ���
		for (unsigned int i = 0; i < good_matches.size(); i++) {
			obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
			scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
		}

		//�������ŷ���任����
		Mat M = estimateRigidTransform(obj, scene, false);
		if (M.data == NULL)
			continue;
		//qDebug("f");
		// cout << "M (default) = " << endl << M << endl <<
		// endl;//������ŷ���任����

		//������ת�Ƕ�
		//�����1�е�2����
		double oneAngle = -asin(M.at<double>(0,1)) / M_PI * 180;
		angleCount++;
		angleSum += oneAngle;
		isSameWheel = true;
		/********compute finish*********/
		
		
		//dstImg[0] = dstImg[1];//drop then dst1, and save the dst2 to dst1//drop the all image//discrete sampling 
	}
}
