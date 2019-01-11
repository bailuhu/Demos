#include "GlobalFunction.h"

#include <QDebug.h>
#include "SearchMark/SearchMark.h"

#pragma comment(lib, "SearchMark/SearchMark.lib")

#define PI 3.1415926

void TraverseDir(QString dirPath, QString filter, vector<QString>& vFilePath)
{  
	QDir dir(dirPath);  

	foreach(QFileInfo fileInfo, dir.entryInfoList())  
	{  
		if(fileInfo.isFile())  
		{  
			
			if (fileInfo.suffix() == filter || fileInfo.fileName().contains(filter))
			{
				qDebug()<< "Find file:" << fileInfo.filePath();
				vFilePath.push_back(fileInfo.filePath());
			}
		}
		else  
		{  
			if(fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;  
			qDebug() << "Find Dir:" << fileInfo.absoluteFilePath();
			TraverseDir(fileInfo.absoluteFilePath(), filter, vFilePath);
		}  
	}  

}  

bool CreateDir(QString dirPath)
{
	
	QDir dir;
	if (dir.exists(dirPath) == true)
	{
		return true;
	}
	else
	{
		int index = 2; // 去掉盘符
		do
		{
			index = dirPath.indexOf("/", index+1);
			QString tempPath = dirPath.left(index);
			if (dir.exists(dirPath) == false)
			{
				if (dir.mkdir(tempPath) == true)
				{
					qDebug() << "Create Dir:" << tempPath;
				}
			}
		}
		while (dir.exists(dirPath) == false);
	}

	

	return true;
}


void goodFeaturesToTrack_Demo(QString filePathR, QString filePathW)
{
	//载入图像，转换为灰度图  
	Mat src = imread(filePathR.toStdString()); 
	Mat dst;
	//为canny边缘图像申请空间，1表示单通道灰度图
	cvtColor(src, dst, CV_BGR2GRAY);

	//初始化 Shi-Tomasi algorithm的一些参数
	vector<Point2f> corners;
	goodFeaturesToTrack(dst, corners, 8, 0.01, 10, Mat(), 78, false, 0.04);
// 	for (int i = 0; i < corners.size(); i++)
// 	{
// 		circle(src, corners[i], 1, Scalar(0, 0, 255), 1, 8, 0);
// 	}

	//精度或最大迭代数目，其中任意一个达到  迭代次数40，精度0.001
	TermCriteria criteria = TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001);
	cornerSubPix(dst, corners, cv::Size(5, 5), cv::Size(-1, -1), criteria);

	//将检测到的亚像素角点绘制到原图上  
	for (int i = 0; i < corners.size(); i++)  
	{  
		Point2f pt1, pt2;
		pt1.x = corners[i].x-5;
		pt1.y = corners[i].y;

		pt2.x = corners[i].x+5;
		pt2.y = corners[i].y;
		line(src, pt1, pt2, Scalar(0, 0, 255));

		pt1.x = corners[i].x;
		pt1.y = corners[i].y-5;

		pt2.x = corners[i].x;
		pt2.y = corners[i].y+5;
		line(src, pt1, pt2, Scalar(0, 0, 255));
	}  

	filePathW.replace(".bmp", "canny.bmp");
	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str();

}


// Image Process

bool SubStrateImg(QString filePathR1, QString filePathR2, QString filePathW)
{
	//载入图像，转换为灰度图  
	Mat src1 = imread(filePathR1.toStdString());
	Mat src2 = imread(filePathR2.toStdString());
	Mat dst = src2 - src1;


	filePathW.replace(".bmp", "GlueShap.bmp");
	imwrite(filePathW.toStdString(), dst);
	qDebug() << filePathW.toStdString().c_str();
	return true;
}

void GrabCutImge(QString filePathR, QString filePathW)
{	
	//载入图像，转换为灰度图  
	Mat src = imread(filePathR.toStdString(), IMREAD_COLOR); 
	Mat dst;
	Mat bgModel, fgModel;
	Rect rect;
	rect.x = 300;
	rect.y = 120;
	rect.width = 800;
	rect.height = 720;
	grabCut(src, dst, rect, bgModel, fgModel, 1, cv::GC_INIT_WITH_RECT);
	compare(dst, cv::GC_PR_FGD, dst, cv::CMP_EQ);   

	//filePathW.replace(".bmp", "GlueShap.bmp");
	imwrite(filePathW.toStdString(), dst);
	qDebug() << filePathW.toStdString().c_str();
}

void EdgeDetect_Contour(QString filePathR, QString filePathW)
{	
	int height = 0;
	int width = 0;

	Mat src = imread(filePathR.toStdString());
	height = src.rows;
	width = src.cols;
	Mat dst;  
	cvtColor(src, dst, CV_BGR2GRAY);  

	GaussianBlur(dst, dst, Size(3, 3), 3, 0);

	threshold(dst, dst, 30, 255, THRESH_BINARY);
	//threshold(dst, dst , 0, 255, CV_THRESH_OTSU);
	// 	imwrite(filePathW.toStdString(), dst);
	// 	qDebug() << filePathW.toStdString().c_str();

	vector<vector<Point> > contours;//contours的类型，双重的vector
	vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// 计算矩  
	vector<Moments> mu(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mu[i] = moments(contours[i], false);  
	}
	///  计算中心矩:  
	vector<Point2f> mc(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
	}  
	/// 绘制轮廓
	double area = 0;
	double maxArea = 0;
	int index = 0;
	for (int i = 0; i < mc.size(); i++)
	{
		Scalar color = Scalar(0,0,255);
		drawContours(src, contours, i, color, 1, 8, hierarchy, 0, Point());
// 		area = contourArea(contours[i]);
// 		if (area > 10000)
// 		{
// 			maxArea = area;
// 			index = i;
// 
// 			// Draw center and rectangle
// 			Scalar color = Scalar(0,255,0);
// 			circle(src, mc[index], 8, color, -1, 8, 0);
// 		}

	}


	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str();
	cout << src;
}

void EdgeDetect_Canny(QString filePathR, QString filePathW)
{

	//载入图像，转换为灰度图  
	Mat src = imread(filePathR.toStdString()); 
	Mat dst;
	//为canny边缘图像申请空间，1表示单通道灰度图
	cvtColor(src, dst, CV_BGR2GRAY);
	Canny( dst, dst, 100, 255, 5, true);//边缘检测

	filePathW.replace(".bmp", "canny.bmp");
	imwrite(filePathW.toStdString(), dst);
	qDebug() << filePathW.toStdString().c_str();
}


int EdgeDetect_Sobel(QString filePathR, QString filePathW)
{
	Mat src, dst, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	int c;
	/// Load an image
	src = imread(filePathR.toStdString());
	if( !src.data )
	{ return -1; }
	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
	/// Convert it to gray
	cvtColor( src, src_gray, CV_RGB2GRAY );
	/// Create window
	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );
	/// Total Gradient (approximate)
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
	//imshow( window_name, grad );
	imwrite(filePathW.toStdString(), grad);
	qDebug() << filePathW.toStdString().c_str();
	return 0;
} 


Point Find_HW_X(QString filePathR, QString filePathW)
{
	Point pt;
	int height = 0;
	int width = 0;

	Mat src = imread(filePathR.toStdString());
	// 2456 x 2054
	// 中心块 160x230 pixel
	height = src.rows;
	width = src.cols;
	{
		Mat dst;  
		cvtColor(src, dst, CV_BGR2GRAY);  
		vector<vector<Point> > contours;//contours的类型，双重的vector
		vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
		GaussianBlur(dst, dst, Size(15, 15), 7, 7);
// 		GaussianBlur(dst, dst, Size(5, 5), 0);
//  	adaptiveThreshold(dst, dst, 255, THRESH_BINARY, 0, 3, 0.5);
		threshold(dst, dst, 30, 255, THRESH_BINARY);
// 		threshold(dst, dst , 0, 255, CV_THRESH_OTSU);
// 		imwrite(filePathW.toStdString(), dst);
// 		qDebug() << filePathW.toStdString().c_str();
		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// 计算矩  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		///  计算中心矩:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
		}  
		/// 绘制轮廓
		double area = 0;
		double maxArea = 0;
		int index = 0;
		for (int i = 0; i < mc.size(); i++)
		{
			Scalar color = Scalar(255);
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
			area = contourArea(contours[i]);
			if (area > maxArea)
			{
				index = i;
			}
		}
		// Draw center and rectangle
		Scalar color = Scalar(0,255,0);
		//circle(src, mc[index], 1, color, -1, 8, 0);
		pt.x = mc[index].x;
		pt.y = mc[index].y;

		rectangle(src, Point(pt.x-1000, pt.y-800), Point(pt.x+1000, pt.y+800), color);
		line(src, Point(0, pt.y), Point(src.cols, pt.y), color);
		line(src, Point(pt.x, 0), Point(pt.x, src.rows), color);

		imwrite(filePathW.toStdString(), src);
		qDebug() << filePathW.toStdString().c_str() << pt.x << pt.y;
	}

	return pt;
}


Point Find_HW_XY(QString filePathR, QString filePathW)
{
	Point pt;
	int ocx = 0, ocX1 = 0, ocX2 = 0;
	int ocy = 0, ocY1 = 0, ocY2 = 0;
	Mat src = imread(filePathR.toStdString());
	Mat dst;
	// find x
	cvtColor(src, dst, CV_BGR2GRAY);  
	
	GaussianBlur(dst, dst, Size(81, 1), 40, 0);
	threshold(dst, dst, 30, 255, THRESH_BINARY);
	vector<vector<Point> > contours;//contours的类型，双重的vector
	vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// 计算矩  
	vector<Moments> mu(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mu[i] = moments(contours[i], false);  
	}
	///  计算中心矩:  
	vector<Point2f> mc(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
	}  
	/// 绘制轮廓
	double area = 0;
	double maxArea = 0;
	int index = 0;
	for (int i = 0; i < mc.size(); i++)
	{
	//	Scalar color = Scalar(255);
	//	drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
		area = contourArea(contours[i]);
		if (area > maxArea)
		{
			index = i;
		}
	}
	ocX1 = mc[index].x;
	ocY1 = mc[index].y;
	circle(src, Point(ocX1,ocY1), 2, Scalar(255), -1, 8, 0);

	// find y
	Mat imgGray,imgBlurHor,imgBinHor, imgGrayHor;
	Mat src2 = imread(filePathR.toStdString());
	Mat dst2;
	cvtColor(src2, dst2, COLOR_BGR2GRAY);
	imgGrayHor = dst2(Rect(dst2.cols / 7*3, 0, dst2.cols / 7, dst2.rows));
	
	GaussianBlur(imgGrayHor, dst, Size(81, 1), 40, 0);
// 	imwrite(filePathW.toStdString(), dst);
// 	qDebug() << filePathW.toStdString().c_str();
	threshold(dst, dst, 50, 255, THRESH_BINARY);
//	threshold(dst, dst, 0, 255, THRESH_OTSU);

	contours.clear();//contours的类型，双重的vector
	hierarchy.clear();//Vec4i是指每一个vector元素中有四个int型数据
	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// 计算矩
	mu.clear();
	mu.resize(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mu[i] = moments(contours[i], false);  
	}
	///  计算中心矩
	mc.clear();
	mc.resize(contours.size());
	for (int i = 0; i < contours.size(); i++)  
	{  
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
	}  
	/// 绘制轮廓
	for (int i = 0; i < mc.size(); i++)
	{
		Scalar color = Scalar(255);
	//	drawContours(src, contours, i, color, 1, 8, hierarchy, 0, Point());
		area = contourArea(contours[i]);
		if (area > 10000)
		{
			maxArea = area;
			index = i;

			// Draw center and rectangle
			Scalar color = Scalar(0,255,255);
		//	circle(src, mc[index], 8, color, -1, 8, 0);
 			line(src, Point(0, mc[index].y), Point(src.cols, mc[index].y), color, 1);
			float minDifY = 0;
			if ((ocX2 - ocX1) < minDifY)
			{
				ocX2 = mc[index].x + src.cols / 7*3;
				ocY2 = mc[index].y;
			}
		}
	}
	
	circle(src, Point(ocX2,ocY2), 2, Scalar(0,255), -1, 8, 0);

	pt.x = ocX1;
	pt.y = ocY2;

// 	line(src, Point(0, ocy), Point(src.cols, ocy), color, 5);
// 	line(src, Point(ocx, 0), Point(ocx, src.rows), color, 5);
	circle(src, Point(ocx,ocy), 2, Scalar(0,0,255), -1, 8, 0);


	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str() << ocx << ocy;;
	return pt;
}


Point Find_HW_XY_2(QString filePathR, QString filePathW)
{
	QString tempPath = filePathW;
	Point pt;
	Mat src = imread(filePathR.toStdString());
	Mat dst;
	Mat roi;
	cvtColor(src, dst, CV_BGR2GRAY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Moments> mu;  
	vector<Point2f> mc;

	// find x from roiHor
	{
		roi = src(Rect(0, src.rows*3/7, src.cols, src.rows/7));
		cvtColor(roi, dst, CV_BGR2GRAY);
		GaussianBlur(dst, dst, Size(51, 51), 25, 25);
		threshold(dst, dst, 30, 255, THRESH_BINARY);

		contours.clear();
		hierarchy.clear();
		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		mu.resize(contours.size());
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		mc.resize(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
		}  
		double area = 0;
		double maxArea = 0;
		int index = 0;
		for (int i = 0; i < mc.size(); i++)
		{
			Scalar color = Scalar(255);
			drawContours(roi, contours, i, color, 2, 8, hierarchy, 0, Point());
			area = contourArea(contours[i]);
			if (area > maxArea)
			{
				index = i;
			}
		}

		circle(roi, Point(mc[index].x,mc[index].y), 2, Scalar(255), -1, 8, 0);

		filePathW = tempPath;
		filePathW.replace(".bmp", "_h.bmp");
		imwrite(filePathW.toStdString(), roi);
		qDebug() << filePathW.toStdString().c_str() << mc[index].x << mc[index].y;
	}
	


	// find y from roiVer
	{
		roi = src(Rect(src.cols*5/11, 0, src.cols/11, src.rows));
		cvtColor(roi, dst, CV_BGR2GRAY);
		GaussianBlur(dst, dst, Size(51, 51), 25, 25);
		threshold(dst, dst, 120, 255, THRESH_BINARY);

		contours.clear();
		hierarchy.clear();
		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		mu.resize(contours.size());
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		mc.resize(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}  
		double area = 0;
		double maxArea = 0;
		int index = 0;
		for (int i = 0; i < mc.size(); i++)
		{
			Scalar color = Scalar(0, 255);
			drawContours(roi, contours, i, color, 2, 8, hierarchy, 0, Point());
			area = contourArea(contours[i]);
			if (area > maxArea)
			{
				index = i;
			}
		}
		circle(roi, Point(mc[index].x,mc[index].y), 2, Scalar(0, 255), -1, 8, 0);

// 		filePathW = tempPath;
// 		filePathW.replace(".bmp", "_v.bmp");
// 		imwrite(filePathW.toStdString(), roi);
// 		qDebug() << filePathW.toStdString().c_str() << mc[index].x << mc[index].y;

	}

	
	return pt;
}


Point OC_Test_MV_2(QString filePathR, QString filePathW)
{
	QString tempPath = filePathW;
	Mat imgDst, imgGray,imgBlurHor,imgBlurVer,imgBinHor,imgBinVer, imgGrayHor, imgGrayVer;
	Mat imgSrc = imread(filePathR.toStdString());
	clock_t startTime, endTime;
	double costTime;
	startTime = clock();
	cvtColor(imgSrc,imgGray,COLOR_RGB2GRAY);
	int ImgSize[2] = { imgGray.rows, imgGray.cols };
	int startY = imgGray.rows / 3;
	int endY = imgGray.rows / 3*2;
	int startX = imgGray.cols / 3;
	int endX = imgGray.cols / 3 * 2;
	imgGrayHor = imgGray(Range(0, imgGray.rows), Range(startX, endX));
	imgGrayVer = imgGray(Range(startY, endY), Range(0, imgGray.cols));
	GaussianBlur(imgGrayHor, imgBlurHor, Size(51, 1), 25, 0);
	GaussianBlur(imgGrayVer, imgBlurVer, Size(1, 51), 0, 25);
	endTime = clock();
	threshold(imgBlurHor, imgBinHor , 0, 255, CV_THRESH_OTSU);
	threshold(imgBlurVer, imgBinVer, 0, 255, CV_THRESH_OTSU);

	filePathW = tempPath;
	filePathW.replace(".bmp", "_blurh.bmp");
	imwrite(filePathW.toStdString(), imgBlurHor);
	filePathW = tempPath;
	filePathW.replace(".bmp", "_blurv.bmp");
	imwrite(filePathW.toStdString(), imgBlurVer);
// 	filePathW = tempPath;
// 	filePathW.replace(".bmp", "_binh.bmp");
// 	imwrite(filePathW.toStdString(), imgBinHor);
// 	filePathW = tempPath;
// 	filePathW.replace(".bmp", "_binv.bmp");
// 	imwrite(filePathW.toStdString(), imgBinVer);

	double sumY = 0;
	double sumP = 0;// Sum of pixel gray
	double sumX = 0;
	double ocY = 0;
	double ocX = 0;
	//%计算质心
	for (int i = 0; i < imgGray.rows; i++) 
	{
		for (int j=0; j < (endX - startX); j++)
		{
			sumY = (i)*imgBinHor.at<uchar>(i, j) + sumY;
			sumP = imgBinHor.at<uchar>(i, j) + sumP;
		}
	}
	ocY = (sumY / sumP);
	sumP = 0;
	for (int i = 0; i < (endY - startY); i++)
	{
		for (int j=0; j < imgGray.cols; j++)
		{
			sumX = (j)*imgBinVer.at<uchar>(i, j) + sumX;
			sumP = imgBinVer.at<uchar>(i, j) + sumP;
		}

	}
	ocX = (sumX / sumP);

	costTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
	// 	cout << "mask中心坐标为：" << ocX << "     " << ocY << endl;
	// 	cout << "运行时间为：" << costTime << endl;
	Point centerPt;//特征点，用以画在图像中    
	centerPt.x = ocX;//特征点在图像中横坐标    
	centerPt.y = ocY;//特征点在图像中纵坐标    
	Scalar clr = Scalar(0, 0, 255);
	circle(imgSrc, centerPt, 10, clr, -1);
	line(imgSrc, Point(0, centerPt.y), Point(imgSrc.cols, centerPt.y), clr);
	line(imgSrc, Point(centerPt.x, 0), Point(centerPt.x, imgSrc.rows), clr);
// 	filePathW = tempPath;
// 	imwrite(filePathW.toStdString(), imgSrc);
// 	qDebug() << filePathW.toStdString().c_str();
	return centerPt;
}


Point OC_Test_MV_4(QString filePathR, QString filePathW)
{
	QString tempPath = filePathW;
	Mat imgDst, imgGray,imgBlurHor,imgBlurVer,imgBinHor,imgBinVer, imgGrayHor, imgGrayVer;
	Mat imgSrc = imread(filePathR.toStdString());
	clock_t startTime, endTime;
	double costTime;
	startTime = clock();
	cvtColor(imgSrc,imgGray,COLOR_RGB2GRAY);
	int ImgSize[2] = { imgGray.rows, imgGray.cols };

	int startY = imgGray.rows / 3;
	int endY = imgGray.rows / 3*2;
	int startX = imgGray.cols / 3;
	int endX = imgGray.cols / 3 * 2;
// 	int startY = imgGray.rows *1/5;
// 	int endY = imgGray.rows *4/5;
// 	int startX = imgGray.cols *1/5;
// 	int endX = imgGray.cols *4/5;
	imgGrayHor = imgGray(Range(0, imgGray.rows), Range(startX, endX));
	imgGrayVer = imgGray(Range(startY, endY), Range(0, imgGray.cols));
// 	GaussianBlur(imgGrayHor, imgBlurHor, Size(51, 1), 25, 0);
// 	GaussianBlur(imgGrayVer, imgBlurVer, Size(1, 51), 0, 25);
	GaussianBlur(imgGrayHor, imgBlurHor, Size(51, 51), 25, 25);
	GaussianBlur(imgGrayVer, imgBlurVer, Size(51, 51), 25, 25);
	endTime = clock();
// 	threshold(imgBlurHor, imgBinHor , 0, 255, CV_THRESH_OTSU);
// 	threshold(imgBlurVer, imgBinVer, 0, 255, CV_THRESH_OTSU);
	threshold(imgBlurHor, imgBinHor , 20, 255, CV_THRESH_BINARY);
	threshold(imgBlurVer, imgBinVer, 20, 255, CV_THRESH_BINARY);

// 	filePathW = tempPath;
// 	filePathW.replace(".bmp", "_blurh.bmp");
// 	imwrite(filePathW.toStdString(), imgBlurHor);
// 	filePathW = tempPath;
// 	filePathW.replace(".bmp", "_blurv.bmp");
// 	imwrite(filePathW.toStdString(), imgBlurVer);
// 
//  filePathW = tempPath;
// 	filePathW.replace(".bmp", "_binh.bmp");
// 	imwrite(filePathW.toStdString(), imgBinHor);
// 	filePathW = tempPath;
// 	filePathW.replace(".bmp", "_binv.bmp");
// 	imwrite(filePathW.toStdString(), imgBinVer);

	double sumY = 0;
	double sumX = 0;
	int cntX = 0;
	int cntY = 0;
	double ocY = 0;
	double ocX = 0;
	//%计算质心
	for (int i = 0; i < imgGray.rows; i++) 
	{
		for (int j=0; j < imgGray.cols; j++)
		{
			// calc y
			if (j < (endX - startX) && imgBinHor.at<uchar>(i, j) > 0)
			{
				sumY = i + sumY;
				cntY++;
			}
			// calc x
			if ( i < (endY - startY) && imgBinVer.at<uchar>(i, j) > 0)
			{
				sumX = j + sumX;
				cntX++;
			}
		}
	}
	ocY = (sumY / cntY);
	ocX = (sumX / cntX);

	costTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
	// 	cout << "mask中心坐标为：" << ocX << "     " << ocY << endl;
	// 	cout << "运行时间为：" << costTime << endl;
	Point centerPt;//特征点，用以画在图像中    
	centerPt.x = ocX;//特征点在图像中横坐标    
	centerPt.y = ocY;//特征点在图像中纵坐标    
	Scalar clr = Scalar(0, 255, 0);
	line(imgSrc, Point(0, centerPt.y), Point(imgSrc.cols, centerPt.y), clr, 2);
	line(imgSrc, Point(centerPt.x, 0), Point(centerPt.x, imgSrc.rows), clr, 2);
	circle(imgSrc, centerPt, 3, Scalar(0, 0, 255), -1);
	filePathW = tempPath;
// 	filePathW.replace(".bmp", "_1.bmp");
	imwrite(filePathW.toStdString(), imgSrc);
	qDebug() << filePathW.toStdString().c_str() << ocX << ocY;
	return centerPt;
}


Point Find_HW_Y(QString filePathR, QString filePathW)
{
	Point pt;
	int height = 0;
	int width = 0;

	Mat src = imread(filePathR.toStdString());
	height = src.rows;
	width = src.cols;
	Mat dst;  
	cvtColor(src, dst, CV_BGR2GRAY);  
	
	GaussianBlur(dst, dst, Size(51, 1), 25, 0);

// 	GaussianBlur(dst, dst, Size(5, 5), 0);
//  adaptiveThreshold(dst, dst, 255, THRESH_BINARY, 0, 3, 0.5);
	threshold(dst, dst, 50, 255, THRESH_BINARY);
// 	threshold(dst, dst , 0, 255, CV_THRESH_OTSU);
// 	imwrite(filePathW.toStdString(), dst);
// 	qDebug() << filePathW.toStdString().c_str();

	vector<vector<Point> > contours;//contours的类型，双重的vector
	vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// 计算矩  
	vector<Moments> mu(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mu[i] = moments(contours[i], false);  
	}
	///  计算中心矩:  
	vector<Point2f> mc(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
	}  
	/// 绘制轮廓
	double area = 0;
	double maxArea = 0;
	int index = 0;
	for (int i = 0; i < mc.size(); i++)
	{
		Scalar color = Scalar(255);
		drawContours(src, contours, i, color, 1, 8, hierarchy, 0, Point());
		area = contourArea(contours[i]);
		if (area > 10000)
		{
			maxArea = area;
			index = i;

			// Draw center and rectangle
			Scalar color = Scalar(0,255,0);
			circle(src, mc[index], 8, color, -1, 8, 0);
			pt.y = mc[index].y;
			line(src, Point(0, pt.y), Point(src.cols, pt.y), color);
		}

	}
	

	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str() << pt.y;;

	return pt;
}



Point OC_Test_MV_3(QString filePathR, QString filePathW)
{
	Mat imgDst, imgGray,imgBlurHor,imgBlurVer,imgBinHor,imgBinVer, imgGrayHor, imgGrayVer;
	Mat imgSrc = imread(filePathR.toStdString());

	cvtColor(imgSrc,imgGray,COLOR_RGB2GRAY);
	int ImgSize[2] = { imgGray.rows, imgGray.cols };
	int startY = imgGray.rows / 7*3;
	int startX = imgGray.cols / 7*3;
	int endY = imgGray.rows / 7*4;
	int endX = imgGray.cols / 7*4;
	imgGrayHor = imgGray(Range(0, imgGray.rows), Range(startX, endX));
	imgGrayVer = imgGray(Range(startY, endY), Range(0, imgGray.cols));
	GaussianBlur(imgGrayHor, imgBlurHor, Size(71, 1), 35, 0);
	GaussianBlur(imgGrayVer, imgBlurVer, Size(1, 71), 0, 35);
	//	GaussianBlur(imgGrayHor, imgBlurHor, Size(71, 71), 35, 35);
	//	GaussianBlur(imgGrayVer, imgBlurVer, Size(71, 71), 35, 35);
	// 	filePathW.replace(".bmp", "_h.bmp");
	// 	imwrite(filePathW.toStdString(), imgBlurHor);
	// 	filePathW.replace("_h.bmp", "_v.bmp");
	// 	imwrite(filePathW.toStdString(), imgBlurVer);
	threshold(imgBlurHor, imgBinHor , 20, 255, CV_THRESH_BINARY);
	threshold(imgBlurVer, imgBinVer, 20, 255, CV_THRESH_BINARY);
	// 	threshold(imgBlurHor, imgBinHor , 0, 255, CV_THRESH_OTSU);
	// 	threshold(imgBlurVer, imgBinVer, 0, 255, CV_THRESH_OTSU);
	// 	adaptiveThreshold(imgBlurHor, imgBinHor, 255, THRESH_BINARY, 0, 3, 0.5);
	// 	adaptiveThreshold(imgBlurVer, imgBinVer, 255, THRESH_BINARY, 0, 3, 0.5);


	double sumY = 0;
	double sumP = 0;// Sum of pixel gray
	double sumX = 0;
	double ocY = 0;
	double ocX = 0;
	//%计算质心
	for (int i = 0; i < imgGray.rows; i++) 
	{
		for (int j=0; j < (endX - startX); j++)
		{
			sumY = (i)*imgBinHor.at<uchar>(i, j) + sumY;
			sumP = imgBinHor.at<uchar>(i, j) + sumP;
		}
	}
	ocY = (sumY / sumP);
	sumP = 0;
	for (int i = 0; i < (endY - startY); i++)
	{
		for (int j=0; j < imgGray.cols; j++)
		{
			sumX = (j)*imgBinVer.at<uchar>(i, j) + sumX;
			sumP = imgBinVer.at<uchar>(i, j) + sumP;
		}

	}
	ocX = (sumX / sumP);

	Point centerPt;//特征点，用以画在图像中    
	centerPt.x = ocX;//特征点在图像中横坐标
	centerPt.y = ocY;//特征点在图像中纵坐标    
	Scalar clr = Scalar(0, 0, 255);
	circle(imgSrc, centerPt, 1, clr, -1);
	line(imgSrc, Point(0, centerPt.y), Point(imgSrc.cols, centerPt.y), clr);
	line(imgSrc, Point(centerPt.x, 0), Point(centerPt.x, imgSrc.rows), clr);
	filePathW.replace("_h.bmp", ".bmp");
	imwrite(filePathW.toStdString(), imgSrc);
	qDebug() << filePathW.toStdString().c_str() << ocX << ocY;
	return centerPt;
}



Mat Gaussian_kernal(int kernel_size, int sigma, int direction = 1)
{
	int m = kernel_size / 2;
	Mat kernel(kernel_size, kernel_size, CV_32FC1);
	float s = 2 * sigma*sigma;
	switch (direction)
	{
	case 1:
		for (int i = 0; i < kernel_size; i++)
		{
			for (int j = 0; j < kernel_size; j++)
			{
				int x = i - m, y=j - m;
				kernel.ptr<float>(i)[j] = exp(-(x*x + y*y) / s) / (PI*s);
			}
		}
		break;
	case 2:
		for (int w = 0; w < kernel_size; w++)
		{
			for (int h = 0; h < kernel_size; h++)
			{
				int x = w - m, y= w - m;
				kernel.ptr<float>(h)[w] = exp(-(x*x + y*y) / s) / (PI*s);
			}
		}
		break;
	default:
		break;
	}
	
	return kernel;
}

void Otsu2Threshold(Mat& src, Mat& dst)
{
	Mat img = src;
	//dst.create(src.size(), src.type());
	CV_Assert(1 == img.channels());

	int height = img.rows;
	int width  = img.cols;
	int Threshold1 = 0;//阈值
	int Threshold2 = 0;//阈值
	double gSum_object;//第一类,前景灰度总值
	double gSum_middle;
	double gSum_blackground;//第二类，背景灰度总值

	double N_object = 0;//前景像素数
	double N_middle = 0;
	double N_blackground = 0;//背景像素数

	double u0 = 0;//前景像素平均灰度
	double u1 = 0;
	double u2 = 0;//背景像素平均灰度
	double w0 = 0;//前景像素点数占整幅图像的比例为ω0
	double w1 = 0;
	double w2 = 0;//背景像素点数占整幅图像的比例为ω1
	double u = 0;//总平均灰度
	double tempg = -1;//临时类间方差
	double g = -1;//类间方差
	double Histogram[256]={0};//灰度直方图
	double N_allpixel = width * height;//总像素数
	for(int k = 0; k < height; k++)
	{
		//计算直方图
		uchar* data = img.ptr<uchar>(k);
		for(int n = 0; n < width; n++)
		{
			Histogram[data[n]]++;
		} 
	}

	// 	Mat Hist = Mat::zeros(256, 256, CV_8UC1);
	// 	//double *tmpHistogram = Histogram;
	// 	for(int k = 0; k < 256; k++)
	// 	{
	// 		//计算直方图
	// 		uchar* data = Hist.ptr<uchar>(k);
	// 		Histogram[k] = Histogram[k]/2773*255;
	// 		for(int n = 0; n < (int)Histogram[k]; n++)
	// 		{
	// 			data[n] = 255;
	// 		} 
	// 	}
	// 	imshow("hist", Hist);



	//计算阈值
	int tmpBlack = 0;
	int tmpMiddle = 0;
	for (int i = 0; i < 256; i++)
	{
		N_object += Histogram[i];
		N_blackground = 0;
		for (int k = i+10; k < 256; k++)
		{
			N_blackground += Histogram[k];
		}
		//if(0 == N_object) continue;//没有前景跳出循环
		for (int j = i+10; j < 256; j++)
		{
			gSum_object = 0;
			gSum_middle = 0;
			gSum_blackground = 0;

			N_middle = N_allpixel - N_object - N_blackground;
			w0 = N_object / N_allpixel;
			w2 = N_blackground / N_allpixel;
			w1 = 1 - w0 - w2;

			//求u0
			for (int k = 0; k <= i; k++)
			{
				gSum_object += k*Histogram[k];
			}
			u0 = gSum_object / N_object;

			//求u1
			for (int k = i+1; k <= j; k++)
			{
				gSum_middle += k*Histogram[k] ;
			}
			u1 = gSum_middle / N_middle;

			//求u2
			for(int k = j+1; k < 256; k++)
			{
				gSum_blackground += k*Histogram[k];
			}
			u2 = gSum_blackground / N_blackground;

			u = w0*u0 + w1*u1 + w2*u2;
			g = w0*(u - u0)*(u - u0) + w1*(u - u1)*(u - u1) + w2*(u - u2)*(u - u2); 

			if (tempg < g)//通过类间方差来修改阈值
			{
				tempg = g;
				Threshold1 = (int)i;
				Threshold2 = (int)j;
				tmpBlack = N_object;
				tmpMiddle = N_middle;
			}

			N_blackground -= Histogram[j];

		}
	}

	int MiddlePixel = 255;
	if (tmpBlack > tmpMiddle)
	{
		MiddlePixel = 0;
	}


	for (int y=0; y < height; y++)
	{ 
		uchar* dstdata = dst.ptr<uchar>(y);
		uchar* srcdata = src.ptr<uchar>(y);
		for (int x=0; x < width; x++)
		{ 

			if ((int)srcdata[x] > Threshold2) 
				dstdata[x] = 255; 
			if ((int)srcdata[x] > Threshold1 && (int)srcdata[x] <= Threshold2) 
				dstdata[x] = MiddlePixel;
			if ((int)srcdata[x] <= Threshold1) 
				dstdata[x] = 0;
		} 
	}


	//黑色背景比例不足40%则认为背景为白，替换为黑背景
	int BlackgroundNum = 0;
	int AllNum = dst.rows * dst.cols;
	for (int i = 0; i < dst.rows; i++)
	{
		uchar* data = dst.ptr<uchar>(i);
		for (int j = 0; j < dst.cols; j++)
		{
			if (data[j] == 0)
			{
				BlackgroundNum++;
			}
		}
	}

	float BlackgroundRatio = 0;
	BlackgroundRatio = (float)BlackgroundNum / (float)AllNum;
	if (BlackgroundRatio < 0.4)
	{
		dst = 255 - dst;
	}
}

int OtsuAlgThreshold(const Mat image)  
{  
    if(image.channels()!=1)  
    {  
        cout<<"Please input Gray-image!"<<endl;  
        return 0;  
    }  
    int T=0; //Otsu算法阈值  
    double varValue=0; //类间方差中间值保存  
    double w0=0; //前景像素点数所占比例  
    double w1=0; //背景像素点数所占比例  
    double u0=0; //前景平均灰度  
    double u1=0; //背景平均灰度  
    double Histogram[256]={0}; //灰度直方图，下标是灰度值，保存内容是灰度值对应的像素点总数  
    uchar *data=image.data;  
    double totalNum=image.rows*image.cols; //像素总数  
    //计算灰度直方图分布，Histogram数组下标是灰度值，保存内容是灰度值对应像素点数  
    for(int i=0;i<image.rows;i++)   //为表述清晰，并没有把rows和cols单独提出来  
    {  
        for(int j=0;j<image.cols;j++)  
        {  
            Histogram[data[i*image.step+j]]++;  
        }  
    }  
    for(int i=0;i<255;i++)  
    {  
        //每次遍历之前初始化各变量  
        w1=0;       u1=0;       w0=0;       u0=0;  
        //***********背景各分量值计算**************************  
        for(int j=0;j<=i;j++) //背景部分各值计算  
        {  
            w1+=Histogram[j];  //背景部分像素点总数  
            u1+=j*Histogram[j]; //背景部分像素总灰度和  
        }  
        if(w1==0) //背景部分像素点数为0时退出  
        {  
            break;  
        }  
        u1=u1/w1; //背景像素平均灰度  
        w1=w1/totalNum; // 背景部分像素点数所占比例  
        //***********背景各分量值计算**************************  
  
        //***********前景各分量值计算**************************  
        for(int k=i+1;k<255;k++)  
        {  
            w0+=Histogram[k];  //前景部分像素点总数  
            u0+=k*Histogram[k]; //前景部分像素总灰度和  
        }  
        if(w0==0) //前景部分像素点数为0时退出  
        {  
            break;  
        }  
        u0=u0/w0; //前景像素平均灰度  
        w0=w0/totalNum; // 前景部分像素点数所占比例  
        //***********前景各分量值计算**************************  
  
        //***********类间方差计算******************************  
        double varValueI=w0*w1*(u1-u0)*(u1-u0); //当前类间方差计算  
        if(varValue<varValueI)  
        {  
            varValue=varValueI;  
            T=i;  
        }  
    }  
    return T;  
}  


void FindCenteroID(Mat& src, QString filePathW, int& x, int& y) 
{
	vector<vector<Point> > contours;//contours的类型，双重的vector
	vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
	findContours(src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// 计算矩  
	vector<Moments> mu(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mu[i] = moments(contours[i], false);  
	}  
	///  计算中心矩:  
	vector<Point2f> mc(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
	}  
	/// 绘制轮廓  
	Mat drawing = Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < contours.size(); i++)  
	{  

		if (contourArea(contours[i]) > 100)
		{
			Scalar color = Scalar(255);  
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());  
			circle(src, mc[i], 4, color, -1, 8, 0);
			x = mc[i].x;
			y = mc[i].y;
		}

	}
	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str();
}


void OC_Test_HW_3(QString filePathR, QString filePathW)
{
	// 6
	int ocX = 0;
	int ocY = 0;
	int height = 0;
	int width = 0;

	Mat src = imread(filePathR.toStdString());
	height = src.rows;
	width = src.cols;
	{
		Mat dst;  
		cvtColor(src, dst, CV_BGR2GRAY);  
		GaussianBlur(dst, dst, Size(81, 81), 40, 40);
		threshold(dst, dst, 30, 255, THRESH_BINARY);

		vector<vector<Point> > contours;//contours的类型，双重的vector
		vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// 计算矩  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		///  计算中心矩:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
		}  
		/// 绘制轮廓
		double area = 0;
		double maxArea = 0;
		int index = 0;
		for (int i = 0; i < mc.size(); i++)
		{
			Scalar color = Scalar(255);
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
			area = contourArea(contours[i]);
			if (area > maxArea)
			{
				index = i;
			}
		}
		// Draw center and rectangle
		Scalar color = Scalar(0,255,0);
		//circle(src, mc[index], 1, color, -1, 8, 0);
		ocX = mc[index].x;
		ocY = mc[index].y;
		//		rectangle(src, Point(ocX-5, ocY-5), Point(ocX+5, ocY+5), color);
		// 		imwrite(filePathW.toStdString(), src);
		// 		qDebug() << filePathW.toStdString().c_str() << ocX << ocY;;
	}
	int w = 20; // edge width
	int h = 20; // edge width
	{
		Mat srcRoi(src, Rect(ocX-w, ocY-h, w*2, h*2));
		Mat dstRoi;
		cvtColor(srcRoi, dstRoi, CV_BGR2GRAY);  
		/// Reduce the noise so we avoid false circle detection
		GaussianBlur( dstRoi, dstRoi, Size(3, 3), 1, 1 );
		threshold(dstRoi, dstRoi, 30, 255, THRESH_BINARY);

		vector<vector<Point> > contours;//contours的类型，双重的vector
		vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
		findContours(dstRoi, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// 计算矩  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		///  计算中心矩:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
		}  
		/// 绘制轮廓
		double area = 0;
		double maxArea = 0;
		int cntX = 0;
		int cntY = 0;
		int maxCntX = 0;
		int maxCntY = 0;
		vector<float> vDis;
		for (int i = 0; i < mc.size(); i++)
		{
			float startX = mc[i].x;
			float startY = mc[i].y;

			area = contourArea(contours[i]);
			if (area < 10) continue;

			// 			cntX = 0;
			// 			for (int j = 0; j < mc.size(); j++)
			// 			{
			// 				float diffX= mc[j].x - startX;
			// 				if (diffX < 1 && diffX > -1)
			// 				{
			// 					cntX++;
			// 				}
			// 			}
			// 			maxCntX = max(cntX, maxCntX);
			// 
			// 			Scalar color = Scalar(255);
			// 			if (cntX >= 11)
			// 			{
			// 				color = Scalar(255);
			// 				drawContours(srcRoi, contours, i, color, 1, 8, hierarchy, 0, Point());
			// 				circle(srcRoi, Point(mc[i].x, mc[i].y) , 1, color, -1);
			// 			}
			//
			// 			cntY = 0;
			// 			for (int j = 0; j < mc.size(); j++)
			// 			{
			// 				int diffY= mc[j].y - startY;
			// 				if (diffY <= 2 && diffY >= -2)
			// 				{
			// 					cntY++;
			// 				}
			// 			}
			// 			maxCntY = max(cntY, maxCntY);
			// 			if (cntY >= maxCntY)
			// 			{
			// 
			// 				color = Scalar(0, 255);
			// 				drawContours(srcRoi, contours, i, color, 1, 8, hierarchy, 0, Point());
			// 
			// 				circle(srcRoi, Point(mc[i].x, mc[i].y) , 1, color, -1);
			// 			}

			qDebug() << area << maxCntX << startX << cntX << "," << startY << cntY;
		}


		imwrite(filePathW.toStdString(), srcRoi);
		qDebug() << filePathW.toStdString().c_str() << ocX << ocY;
	}
	return ;
}


void OC_Test_HW_4(QString filePathR, QString filePathW)
{
	int ocX = 0;
	int ocY = 0;
	int height = 0;
	int width = 0;

	Mat src = imread(filePathR.toStdString());
	height = src.rows;
	width = src.cols;
	{
		Mat dst;  
		cvtColor(src, dst, CV_BGR2GRAY);  
		GaussianBlur(dst, dst, Size(81, 81), 40, 40);
		threshold(dst, dst, 30, 255, THRESH_BINARY);

		vector<vector<Point> > contours;//contours的类型，双重的vector
		vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// 计算矩  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		///  计算中心矩:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
		}  
		/// 绘制轮廓
		double area = 0;
		double maxArea = 0;
		int index = 0;
		for (int i = 0; i < mc.size(); i++)
		{
			Scalar color = Scalar(255);
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
			area = contourArea(contours[i]);
			if (area > maxArea)
			{
				index = i;
			}
		}
		// Draw center and rectangle
		// Draw center and rectangle
		Scalar color = Scalar(0,255,0);
		//circle(src, mc[index], 1, color, -1, 8, 0);
		ocX = mc[index].x;
		ocY = mc[index].y;
		// 		rectangle(src, Point(ocX-20, ocY-40), Point(ocX+20, ocY+40), color);
		// 		imwrite(filePathW.toStdString(), src);
		// 		qDebug() << filePathW.toStdString().c_str() << ocX << ocY;;
	}

	int w = 100; // edge width
	int h = 200; // edge width
	Mat srcRoi(src, Rect(ocX-w, ocY-h, w*2, h*2));
	{
		Mat srcRoi(src, Rect(ocX-w, ocY-h, w*2, h*2));
		Mat dstRoi;
		cvtColor(srcRoi, dstRoi, CV_BGR2GRAY);  
		GaussianBlur(dstRoi, dstRoi, Size(1, 81), 0, 40);
		//	imwrite(filePathW.toStdString(), dstRoi);
		threshold(dstRoi, dstRoi, 0, 255, CV_THRESH_OTSU);

		double sumP = 0;// Sum of pixel gray
		double sumY = 0;
		// Calc weight center
		for (int i = 0; i < dstRoi.rows; i++) 
		{
			for (int j=0; j < dstRoi.cols; j++)
			{
				sumY = (i)*dstRoi.at<uchar>(i, j) + sumY;
				sumP = dstRoi.at<uchar>(i, j) + sumP;
			}
		}
		double tempY = (sumY / sumP);
		ocY += tempY - h;
	}

	w = 200; // edge width
	h = 100; // edge width
	{
		Mat srcRoi(src, Rect(ocX-w, ocY-h, w*2, h*2));
		Mat dstRoi;
		cvtColor(srcRoi, dstRoi, CV_BGR2GRAY);  
		GaussianBlur(dstRoi, dstRoi, Size(81, 1), 40, 0);
		threshold(dstRoi, dstRoi, 0, 255, CV_THRESH_OTSU);
		imwrite(filePathW.toStdString(), dstRoi);
		double sumP = 0;// Sum of pixel gray
		double sumX = 0;
		for (int i = 0; i < dstRoi.rows; i++)
		{
			for (int j=0; j < dstRoi.cols; j++)
			{
				sumX = (j)*dstRoi.at<uchar>(i, j) + sumX;
				sumP = dstRoi.at<uchar>(i, j) + sumP;
			}

		}
		double tempX = (sumX / sumP);
		ocX += tempX - w;
	}

	Scalar color = Scalar(0, 255);
	rectangle(src, Point(ocX-125, ocY-100), Point(ocX+125, ocY+100), color);
	circle(src, Point(ocX, ocY) , 5, color, -1);
	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str() << ocX << ocY;

	return ;
}

void OC_Test_HW_1(QString filePathR, QString filePathW)
{
	// 6
	int ocX = 0;
	int ocY = 0;
	Mat src = imread(filePathR.toStdString());
	int height = src.rows;
	int width = src.cols;
	int w = 500;
	int h = 500;
	{
		Mat dst;  
		cvtColor(src, dst, CV_BGR2GRAY);  
		vector<vector<Point> > contours;//contours的类型，双重的vector
		vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
		GaussianBlur(dst, dst, Size(5, 5), 0);
		// 		adaptiveThreshold(dst, dst, 255, THRESH_BINARY, 0, 5, 0.5);
		threshold(dst, dst, 30, 255, THRESH_BINARY);
		//		threshold(dst, dst , 0, 255, CV_THRESH_OTSU);
		// 		imwrite(filePathW.toStdString(), dst);
		// 		qDebug() << filePathW.toStdString().c_str();
		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// 计算矩  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		///  计算中心矩:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
		}  
		/// 绘制轮廓
		double area = 0;
		double maxArea = 0;
		int index = 0;
		for (int i = 0; i < mc.size(); i++)
		{
			Scalar color = Scalar(255);
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
			area = contourArea(contours[i]);
			if (area > maxArea)
			{
				index = i;
			}
		}
		Scalar color = Scalar(0,255,0);
		circle(src, mc[index], 4, color, -1, 8, 0);
		ocX = mc[index].x;
		ocY = mc[index].y;
		rectangle(src, Point(ocX-1000, ocY-800), Point(ocX+1000, ocY+800), color);

		imwrite(filePathW.toStdString(), src);
		qDebug() << filePathW.toStdString().c_str();
	}
	return ;

	{
		Mat src = imread(filePathR.toStdString());
		Mat dst;  
		cvtColor(src, dst, CV_BGR2GRAY);
		Mat roi(dst, Rect(ocX-w, ocY-h, w*2, h*2));
		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
		erode(roi, roi, element);//腐蚀操作
		dilate(roi, roi, element);//膨胀操作
		vector<vector<Point> > contours;//contours的类型，双重的vector
		vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
		GaussianBlur(roi, roi, Size(5, 5), 0);
		threshold(roi, roi, 20, 255, THRESH_BINARY_INV);
		imwrite(filePathW.toStdString(), src);
		qDebug() << filePathW.toStdString().c_str();
		findContours(roi, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// 计算矩  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);
		}  
		///  计算中心矩:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);

		}
		/// 绘制轮廓
		for (int i = 0; i < mc.size(); i++)
		{

			mc[i].x = ocX - w + mc[i].x;
			mc[i].y = ocY - h + mc[i].y;
			Scalar color = Scalar(255);
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
			color = Scalar(0,0,255);
			circle(src, mc[i], 4, color, -1, 8, 0);

			double area = contourArea(contours[i]);
			if (area > 20000)
			{
				Scalar color = Scalar(0,255,0);
				circle(src, mc[i], 4, color, -1, 8, 0);
			}
		}
		imwrite(filePathW.toStdString(), src);
		qDebug() << filePathW.toStdString().c_str();
	}
}


void ClockWiseRotate(QString filePathR, QString filePathW)
{
	Mat src = imread(filePathR.toStdString());
	//flipCode: 翻转模式，
	// ==0 垂直翻转（沿X轴翻转）
	// > 0 水平翻转（沿Y轴翻转）
	// < 0 水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
	//clockwise 90
	transpose(src, src);
	flip(src, src, 1);
	imwrite(filePathW.toStdString(), src);

	//clockwise 180
	// 	flip(src, src, -1);
	// 	imwrite(filePathW.toStdString(), src);

	//clockwise 270
	// 	transpose(src, src);
	// 	flip(src, src, 0);
	// 	imwrite(filePathW.toStdString(), src);
}
// 
// bool HuaweiDllCall()
// {
// 	char cTemp[MAX_PATH];
// 	sprintf_s(cTemp, "E:/xx/x/xNicolas/OpencvDemo/AA_Single/Huawei/Proj_IS.dll");
// 
// 	HMODULE hdll = LoadLibraryA(cTemp);
// 	if (hdll)
// 	{
// 		qDebug() << "LoadLibrary OK";
// 		//定义函数指针
// 		FARPROC fpFun = GetProcAddress(hdll, "dfSpotTestCreate");
// 		//执行dll接口函数
// 		if (!fpFun)
// 		{
// 
// 		}
// 
// 	}
// 	else
// 	{
// 		qDebug() << "LoadLibrary NG";
// 		//	DWORD dwErr = GetLastError();
// 		LPVOID lpMsgBuf;
// 		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
// 			NULL, GetLastError(), 0, (LPSTR)&lpMsgBuf, 0, NULL);
// 
// 		char cTemp[MAX_PATH];
// 		sprintf_s(cTemp, "%s", lpMsgBuf);
// 		qDebug() << cTemp;
// 	}
// 	system("pause");
// 	return 0;
// }

void MophologyOpenClose(QString filePathR, QString filePathW)
{
	Mat img = imread(filePathR.toStdString());
	// 	namedWindow("原始图", WINDOW_NORMAL);
	// 	imshow("原始图", img);
	//获取自定义核
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	Mat img1,img2;
	// 	dilate(img, img1, element);//膨胀操作
	// 	erode(img1, img2, element);//腐蚀操作
	// 	namedWindow("腐蚀操作", WINDOW_NORMAL);
	// 	imshow("腐蚀操作", img2);

	Mat out;
	morphologyEx(img, img1, MORPH_CLOSE, element);
	// 	namedWindow("MORPH_CLOSE", WINDOW_NORMAL);
	// 	imshow("MORPH_CLOSE", img1);
	filePathW.replace(".bmp", "-close.bmp");
	imwrite(filePathW.toStdString(), img1);
	qDebug() << filePathW.toStdString().c_str();
	filePathW.replace("-close.bmp", ".bmp");

	morphologyEx(img, img2, MORPH_OPEN, element);
	// 	namedWindow("MORPH_OPEN", WINDOW_NORMAL);
	// 	imshow("MORPH_OPEN", img2);
	filePathW.replace(".bmp", "-open.bmp");
	imwrite(filePathW.toStdString(), img2);
	qDebug() << filePathW.toStdString().c_str();
}


void LineDetect_NoName(QString filePathR, QString filePathW)
{

	//载入图像，转换为灰度图  
	Mat src = imread(filePathR.toStdString()); 
	Mat dst;
	//为canny边缘图像申请空间，1表示单通道灰度图
	cvtColor(src, dst, CV_BGR2GRAY);
	/// Reduce the noise so we avoid false circle detection
	//threshold(dst, dst, 30, 255, THRESH_BINARY);
	threshold(dst, dst , 0, 255, CV_THRESH_OTSU);
	imwrite(filePathW.toStdString(), dst);
	qDebug() << filePathW.toStdString().c_str();
}


void LineDetect_HT(QString filePathR, QString filePathW)
{

	//载入图像，转换为灰度图  
	Mat src = imread(filePathR.toStdString()); 
	Mat dst;
	//为canny边缘图像申请空间，1表示单通道灰度图
	//	Canny(src, dst, 128, 255);
	cvtColor(src, dst, CV_BGR2GRAY);
	threshold(dst, dst, 60, 255, THRESH_BINARY_INV);

	//	cvtColor(src, dst, CV_BGR2GRAY);
	LineFinder lf;
	lf.setLineLengthAndGap(50, 5);
	lf.setMinVote(80);
	vector<Vec4i> lines = lf.findLines(dst);
	lf.drawDetectLines(src);
	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str();
}

void Mophology_WaterShed(QString filePathR, QString filePathW)
{
	Mat src = imread(filePathR.toStdString());
	Mat dst;
	cvtColor(src, dst, CV_BGR2GRAY);

	//	medianBlur(dst, dst, 7);
	GaussianBlur(dst, dst, Size(51, 51), 25, 25);

	threshold(dst, dst, 20, 255, THRESH_BINARY);
	imwrite(filePathW.toStdString(), dst);

	vector<vector<Point> > contours;//contours的类型，双重的vector
	vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// 计算矩  
	vector<Moments> mu(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mu[i] = moments(contours[i], false);
	}  
	///  计算中心矩:  
	vector<Point2f> mc(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);

	}
	/// 绘制轮廓
	for (int i = 0; i < mc.size(); i++)
	{
		Scalar color = Scalar(255);
		drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	//	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str();
}

void HarrisDectect_1(QString filePathR, QString filePathW)
{
	Mat src = imread(filePathR.toStdString());
	Mat dst;
	cvtColor(src, dst, CV_BGR2GRAY);
	HarrisDetector harris;
	harris.detect(dst);
	vector<Point> vPt;
	harris.getCorners(vPt, 0.01);
	harris.drawOnImage(dst, vPt);

	imwrite(filePathW.toStdString(), dst);
	qDebug() << filePathW.toStdString().c_str();

}

Point FindCircle(QString filePathR, QString filePathW)
{
	Mat src = imread(filePathR.toStdString());
	Mat dst;

	cvtColor(src, dst, CV_BGR2GRAY);

	//GaussianBlur(dst, dst, Size(9, 9), 2, 2);
// 	for (int i = 0; i < dst.rows; i++) 
// 	{
// 		for (int j=0; j < dst.cols; j++)
// 		{
// 			int valGray = dst.at<uchar>(i, j);
// 		//	if ( valGray > 50 && valGray < 230)
// 			if ( valGray > 25 && valGray < 30)
// 			{
// 				dst.at<uchar>(i, j) = 255;
// 			}
// 			else
// 			{
// 				dst.at<uchar>(i, j) = 0;
// 			}
// 		}
// 	}
	SearchMark::BlockInfo bi;
	int left = 1900;
	int top = 150;
	int width = 500;
	int height = 500;
	int right = left + width;
	int bottom = top + height;

	SearchMark::GetTargetInfo(dst.data, dst.cols, dst.rows, left, top, right, bottom, true, 25, bi);
	rectangle(src, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 3);
	circle(src, Point(bi.x, bi.y), 3, Scalar(0, 255, 0), -1, 8, 0);

	left = 400;
	top = 1200;
	right = left + width;
	bottom = top + height;
	SearchMark::GetTargetInfo(dst.data, dst.cols, dst.rows, left, top, right, bottom, true, 25, bi);
	rectangle(src, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 3);
	circle(src, Point(bi.x, bi.y), 3, Scalar(0, 255, 0), -1, 8, 0);

	left = 1750;
	top = 2400;
	right = left + width;
	bottom = top + height;
	SearchMark::GetTargetInfo(dst.data, dst.cols, dst.rows, left, top, right, bottom, true, 25, bi);
	rectangle(src, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 3);
	circle(src, Point(bi.x, bi.y), 3, Scalar(0, 255, 0), -1, 8, 0);

	left = 3150;
	top = 1400;
	right = left + width;
	bottom = top + height;
	SearchMark::GetTargetInfo(dst.data, dst.cols, dst.rows, left, top, right, bottom, true, 25, bi);
	rectangle(src, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 3);
	circle(src, Point(bi.x, bi.y), 3, Scalar(0, 255, 0), -1, 8, 0);

// 	threshold(dst, dst, 30, 255, THRESH_BINARY);
// 	threshold(dst, dst, 0, 255, THRESH_OTSU);
//	adaptiveThreshold(dst, dst, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 3, 0);
//	adaptiveThreshold(dst, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 3, 0);
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	imwrite(filePathW.toStdString(), src, compression_params);
	qDebug() << filePathW.toStdString().c_str();
	return 0;
	vector<Vec3f> circles;
	HoughCircles(dst, circles, CV_HOUGH_GRADIENT, 1.5, 500, 100, 100);
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle(src, center, 2, Scalar(0, 255, 0), -1, 8, 0);
		circle(src, center, radius, Scalar(0, 0, 255), 2, 8, 0);
	}
	
	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str();
	return 0;
}