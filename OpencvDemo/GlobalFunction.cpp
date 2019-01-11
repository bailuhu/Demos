#include "GlobalFunction.h"

#include <QDebug.h>

void TraverseDir(QString dirPath, QString filter, vector<QString>& vFilePath)
{  
	QDir dir(dirPath);  

	foreach(QFileInfo fileInfo, dir.entryInfoList())  
	{  
		if(fileInfo.isFile())  
		{  
			
			if (fileInfo.suffix() == filter)
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




// Image Process

void EdgeDetect_Canny(QString filePathR, QString filePathW)
{

	//载入图像，转换为灰度图  
	Mat src = imread(filePathR.toStdString()); 
	Mat dst;
	//为canny边缘图像申请空间，1表示单通道灰度图
	cvtColor(src, dst, CV_BGR2GRAY);
	Canny( dst, dst, 20, 255, 5, true);//边缘检测

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
//  	filePathW = tempPath;
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
