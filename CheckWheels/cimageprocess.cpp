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
	//-----------------------------------【全局变量声明部分】--------------------------------------
	//参数调节
	int gs1 = 7;//越大越快，准确率越低
	//霍夫圆检测时高斯滤波内核大小，只能为奇数，越大越模糊，圆与背景区分不明显时，减小高斯值
	double dp = 1;//圆心累加器图像分辨率与输入图像之比的倒数，值越小，代表累加器分辨率越高，识别圆会更加慢
	double minDist = 90;//霍夫变换检测到的圆的圆心之间的最小距离，值越大，某些圆会检测不出来，值越小，多个相邻的圆会被错误的检测成一个重合的圆
	double param1 = 300;//传递给canny边缘检测算子的高阈值，且低阈值为高阈值的一半
	double param2 = 60;//50~80大了识别不到，小了找到很多圆。越大越快
	//检测阶段圆心累加器的阈值，值越大，检测到的圆越完美，值越小，可以检测到更多根本不存在的圆
	//double s = 1.5;//特征点匹配距离与最小距离的比,如果一直不现实8，可适当增大该值，减小gs2
	int gs2 = 9;//特征匹配时高斯滤波内核大小，只能为奇数，越大越模糊，增大高斯值，可使特征匹配时集中在主要特征处

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
		//********************霍夫圆检测并分割圆环**************************//
		GaussianBlur(roiImage, blurImage, Size(gs1, gs1), 2, 2);//修改内核大小可更改识别圆的难易，越小越容易
		vector<Vec3f> circles;
		//霍夫圆
		HoughCircles(blurImage, circles, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, 200, 300); //可修改参数调节捕捉圆的准确度
		//if (circles.size() < minCycleSize || circles.size() > maxCycleSize) //is it diameter or radium?
		if (circles.size() < 1)
		{
		//	qDebug("No cycle:%d\n", ++iImgNoCycle);
			iImgCount = 0;
			oneWheel();
			continue;
		}
		/*judge if the circle is out of ROI*/
		Point center0(cvRound(circles[0][0]), cvRound(circles[0][1])); //圆环中心
		int radiusOutside = cvRound(circles[0][2]) - 10;	//圆环外径
		int radiusInside = radiusOutside / 1.4; //圆环内径
		
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

	
		/*********************遍历法效率非常低（80ms）*********************/
		/*Mat imageRing(2 * radiusOutside + 1, 2 * radiusOutside + 1, roiImage.type()); //定义圆环,最外圈加框

		for (int ringX = 0; ringX < imageRing.cols; ringX++) {
			for (int ringY = 0; ringY < imageRing.rows; ringY++) {
				int roiX = ringX + center0.x - radiusOutside ; //两幅图像进行坐标变换
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
		//将两次得到的圆环图像调整为同样大小
		//resize the first dstImg same as the second
		//so that radius is right
		
		resize(dstImg[0], dstImg[0], Size(dstImg[1].cols, dstImg[1].rows), 0, 0, CV_INTER_LINEAR);

		//imshow("Ring", dstImg[1]);

		//**************************图像特征匹配部分*****************************************//

		//cvtColor(dstImg[0], dstImg[0], CV_BGR2GRAY); //将切割后的图片转化为灰度图
		//cvtColor(dstImg[1], dstImg[1], CV_BGR2GRAY);
		//GaussianBlur(dstImg[0], dstImg[0], Size(7, 7), 2, 2); //修改内核大小可更改识别圆的难易，越小越容易
		//GaussianBlur(dstImg[1], dstImg[1], Size(7, 7), 2, 2);
		
		/******************ORB算法*********************/
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
		
		//暴力匹配
		Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
		
		//Lowe's algorithm
		//劳氏算法选出优秀的匹配
		vector< vector<DMatch> > matches;
		matcher->knnMatch(descriptors1, descriptors2, matches, 2);
		vector<DMatch> good_matches;
		//计算出关键点之间距离的最大值和最小值
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
		//使用SURF算子检测关键点
		int minHessian = 400; // SURF算法中的hessian阈值
		Ptr<SURF> detector = SURF::create(minHessian);//定义一个SurfFeatureDetector（SURF） 特征检测类对象
		//SurfFeatureDetector detector(minHessian);
		vector<KeyPoint> keypoints_object, keypoints_scene; // vector模板类，存放任意类型的动态数组

		//调用detect函数检测出SURF特征关键点，保存在vector容器中
		detector->detect(dstImg[0], keypoints_object);
		detector->detect(dstImg[1], keypoints_scene);

		//计算描述符（特征向量）
		//SurfDescriptorExtractor extractor;
		Mat descriptors_object, descriptors_scene;
		detector->compute(dstImg[0], keypoints_object, descriptors_object);
		detector->compute(dstImg[1], keypoints_scene, descriptors_scene);

		//使用FLANN匹配算子进行匹配
		FlannBasedMatcher matcher;
		vector<DMatch> matches;
		matcher.match(descriptors_object, descriptors_scene, matches);
		double max_dist = 0;
		double min_dist = 100; //最小距离和最大距离
		
		//计算出关键点之间距离的最大值和最小值
		for (int i = 0; i < descriptors_object.rows; i++) {
			double dist = matches[i].distance;
			if (dist < min_dist)
				min_dist = dist;
			if (dist > max_dist)
				max_dist = dist;
		}
		

		//存下匹配距离小于1.5*min_dist的点对 //越低越精确
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

		//判断有效匹配点对是否大于等于10个
		if (good_matches.size() < 3)
		{
			//qDebug("the match point less than 3");
			continue;
		}
		

		//绘制出匹配到的关键点
		Mat image_matches;
		drawMatches(dstImg[0], keypoints1, dstImg[1], keypoints2,
			good_matches, image_matches, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		//显示最终结果
 		imshow("特征点匹配图", image_matches);
		//qDebug() << "match" << time.elapsed() / 1000.0 << "s" << endl;
		waitKey(30);

		

		//***********************计算放射变换矩阵，计算旋转角度**************************//
		//定义两个局部变量
		vector<Point2f> obj;
		vector<Point2f> scene;

		//从匹配成功的匹配对中获取关键点
		for (unsigned int i = 0; i < good_matches.size(); i++) {
			obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
			scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
		}

		//计算最优放射变换矩阵
		Mat M = estimateRigidTransform(obj, scene, false);
		if (M.data == NULL)
			continue;
		//qDebug("f");
		// cout << "M (default) = " << endl << M << endl <<
		// endl;//输出最优放射变换矩阵

		//计算旋转角度
		//输出第1行第2个数
		double oneAngle = -asin(M.at<double>(0,1)) / M_PI * 180;
		angleCount++;
		angleSum += oneAngle;
		isSameWheel = true;
		/********compute finish*********/
		
		
		//dstImg[0] = dstImg[1];//drop then dst1, and save the dst2 to dst1//drop the all image//discrete sampling 
	}
}
