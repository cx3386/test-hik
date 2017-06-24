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
	//-----------------------------------【全局变量声明部分】--------------------------------------
	//参数调节
	int gs1 = 7;//霍夫圆检测时高斯滤波内核大小，只能为奇数，越大越模糊，圆与背景区分不明显时，减小高斯值
	double dp = 1.5;//圆心累加器图像分辨率与输入图像之比的倒数，值越小，代表累加器分辨率越高，识别圆会更加慢
	double minDist = 20;//霍夫变换检测到的圆的圆心之间的最小距离，值越大，某些圆会检测不出来，值越小，多个相邻的圆会被错误的检测成一个重合的圆
	double param1 = 200;//传递给canny边缘检测算子的高阈值，且低阈值为高阈值的一半
	double param2 = 100;//检测阶段圆心累加器的阈值，值越大，检测到的圆越完美，值越小，可以检测到更多根本不存在的圆
	double s = 3;//特征点匹配距离与最小距离的比,如果一直不现实8，可适当增大该值，减小gs2
	int gs2 = 9;//特征匹配时高斯滤波内核大小，只能为奇数，越大越模糊，增大高斯值，可使特征匹配时集中在主要特征处

	Mat dstImg;

	while (!quit) {
		// wait until a image need process
		mutex.lock();
		imageNeedProcess.wait(&mutex);
		dstImg = pRawImage;
		mutex.unlock();

		//********************霍夫圆检测并分割圆环**************************//
		cv::GaussianBlur(dstImg, dstImg, Size(gs1, gs1), 2, 2);//修改内核大小可更改识别圆的难易，越小越容易
		vector<Vec3f> circles;
		//霍夫圆
		HoughCircles(dstImg, circles, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, 0, 0); //可修改参数调节捕捉圆的准确度
		if (circles.size() < 1) {
			qDebug("第%d次未识别到圆\n",++iImgNoCycle);
			continue; 
		}
		//进行第一次圆图像分割
		Point center0(cvRound(circles[0][0]),
			cvRound(circles[0][1])); //定义第二次圆分割半径及圆心
		int radius0 = cvRound(circles[0][2]) - 10;
		//绘制圆心
		// circle(imagetemp, center0, 3, Scalar(0, 255, 0), -1, 8, 0);
		//绘制圆轮廓
		// circle(imagetemp, center0, radius0, Scalar(155, 50, 255), 3, 8, 0);
		imshow("捕捉效果图", imagetemp0);
		
		qDebug("5");
		Mat imagetemp1(imagetemp0.rows, imagetemp0.cols,
			imagetemp0.type()); //定义第一次切割后图像矩阵
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
		//进行第二次圆图像分割
		int radius1 = radius0 / 2 - 10; //定义第二次圆分割半径半径
		Mat imagetemp2(imagetemp1.rows, imagetemp1.cols,
			imagetemp1.type()); //定义分割后图像
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
		//将分割后图像大小调整为圆环大小
		Mat imagetemp3(2 * radius0, 2 * radius0,
			imagetemp0.type()); //定义调整后图像
		for (int x = 0; x < imagetemp2.cols; x++) {
			for (int y = 0; y < imagetemp2.rows; y++) {
				int temp = ((x - center0.x) * (x - center0.x) +
					(y - center0.y) * (y - center0.y));
				int tempx = x - center0.x + radius0; //两幅图像进行坐标变换
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

//将两次得到的圆环图像调整为同样大小
Mat imagetemp4;
resize(outimage[1], imagetemp4, Size(outimage[0].cols, outimage[0].rows), 0,
	0, CV_INTER_LINEAR);
outimage[1] = imagetemp4;

//**************************图像特征匹配部分*****************************************//

cvtColor(outimage[0], outimage[0],
	CV_BGR2GRAY); //将切割后的图片转化为灰度图
cvtColor(outimage[1], outimage[1], CV_BGR2GRAY);
GaussianBlur(outimage[0], outimage[0], Size(7, 7), 2,
	2); //修改内核大小可更改识别圆的难易，越小越容易
GaussianBlur(outimage[1], outimage[1], Size(7, 7), 2, 2);
// imshow("第一幅图切割效果图", outimage[0]);
// imshow("第二幅图切割效果图", outimage[1]);

//使用SURF算子检测关键点
int minHessian = 400; // SURF算法中的hessian阈值
SurfFeatureDetector detector(
	minHessian); //定义一个SurfFeatureDetector（SURF） 特征检测类对象
vector<KeyPoint> keypoints_object,
keypoints_scene; // vector模板类，存放任意类型的动态数组

//调用detect函数检测出SURF特征关键点，保存在vector容器中
detector.detect(outimage[0], keypoints_object);
detector.detect(outimage[1], keypoints_scene);

//计算描述符（特征向量）
SurfDescriptorExtractor extractor;
Mat descriptors_object, descriptors_scene;
extractor.compute(outimage[0], keypoints_object, descriptors_object);
extractor.compute(outimage[1], keypoints_scene, descriptors_scene);

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

//存下匹配距离小于1.5*min_dist的点对
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

//判断有效匹配点对是否大于等于3个
if (good_matches.size() < 3)
	continue;
qDebug("8");

//绘制出匹配到的关键点
Mat image_matches;
drawMatches(outimage[0], keypoints_object, outimage[1], keypoints_scene,
	good_matches, image_matches, Scalar::all(-1), Scalar::all(-1),
	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

//显示最终结果
imshow("特征点匹配图", image_matches);
qDebug("9");

//***********************计算放射变换矩阵，计算旋转角度**************************//
//定义两个局部变量
vector<Point2f> obj;
vector<Point2f> scene;

//从匹配成功的匹配对中获取关键点
for (unsigned int i = 0; i < good_matches.size(); i++) {
	obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
	scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
}

//计算最优放射变换矩阵
Mat M = estimateRigidTransform(obj, scene, false);
if (M.data == NULL)
continue;
qDebug("f");
// cout << "M (default) = " << endl << M << endl <<
// endl;//输出最优放射变换矩阵

//计算旋转角度
double* data = M.ptr<double>(0); //访问数组地址
double t = data[1];              //输出第1行第2个数
double f = -asin(t) / 3.1415926 * 180;
qDebug("车轮顺时针转动%lf度\n", f);
	}
}
}
