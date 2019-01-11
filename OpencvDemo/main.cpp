#include <QtWidgets/QApplication>

#include "QDlgMain.h"
#include "GlobalFunction.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;
//全局变量
Mat src1, src1_gray,
	src2, src2_gray;
int maxCorners = 23;
int maxTrackbar = 100;
double qualityLevel = 0.01;//最小特征值小于qualityLevel*最大特征值的点将被忽略
int blockSize = 3;//邻域尺寸
double minDistance = 10;//两角点间最小距离
char* source_window1 = "src1";
char* source_window2 = "src2";
void goodFeaturesToTrack_Demo( int, void* )
{
	//参数初始化
	if(maxCorners < 1)//允许返回的最多角点个数
		maxCorners = 1;
	vector<Point2f> corners1,corners2;//角点容器
	
	

	bool useHarrisDetector = false;//是否使用Harris
	double k = 0.04;
	//拷贝原图
	Mat src1_copy = src1.clone();
	//调用函数进行Shi-Tomasi角点检测
	goodFeaturesToTrack( src1_gray,
		corners1,
		maxCorners,
		qualityLevel,
		minDistance,
		Mat(),
		blockSize,
		useHarrisDetector,
		k );
	goodFeaturesToTrack( src2_gray,
		corners2,
		maxCorners,
		qualityLevel,
		minDistance,
		Mat(),
		blockSize,
		useHarrisDetector,
		k );
	//画出角点
	cout<<"角点个数："<<corners1.size()<<endl;//等于maxCorners
	for( int i = 0; i < corners1.size(); i++ )
	{
		circle( src1_copy, corners1[i], 4, Scalar(0,255,0),2);
	}
	//显示图像
	imshow( source_window1, src1_copy );
}
// int main()
// {
// 	//加载源图并转换为灰度图
// 	src1 = imread("E:/Test/1.bmp");
// 	cvtColor( src1, src1_gray, CV_BGR2GRAY );
// 	//创建窗口
// 	namedWindow( source_window1, CV_WINDOW_AUTOSIZE );
// 	//创建滑块条，调节允许的角点个数
// 	createTrackbar( "BlockSize:", source_window1, &blockSize, maxTrackbar, goodFeaturesToTrack_Demo );
// 	createTrackbar( "MaxCorner:", source_window1, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo );
// 	goodFeaturesToTrack_Demo( 0, 0 );
// 	waitKey(0);
// 	return(0);
// }



int main(int argc, char *argv[])
{
#if 1

// 单张图调试参数
// 	//加载源图并转换为灰度图
// 	src1 = imread("E:/Test/2.bmp");
// 	cvtColor( src1, src1_gray, CV_BGR2GRAY );
// 	//创建窗口
// 	namedWindow( source_window1, CV_WINDOW_AUTOSIZE );
// 	//创建滑块条，调节允许的角点个数
// 	createTrackbar( "BlockSize:", source_window1, &blockSize, maxTrackbar, goodFeaturesToTrack_Demo );
// 	createTrackbar( "MaxCorner:", source_window1, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo );
// 	goodFeaturesToTrack_Demo( 0, 0 );
// 	waitKey(0);
// 	return(0);
	QString temp;
	for (int n = 1; n <= 15; n++)
	{
		// 图像相减
		temp = QString::fromLatin1("E:/Test/%1/yyy.bmp").arg(n);
		Mat img1 = imread(temp.toStdString().c_str()); 
		temp = QString::fromLatin1("E:/Test/%1/jjj.bmp").arg(n);
		Mat img2 = imread(temp.toStdString().c_str());
// 		GaussianBlur( img1, img1, Size(3,3), 1, 1, BORDER_DEFAULT );
// 		GaussianBlur( img2, img2, Size(3,3), 1, 1, BORDER_DEFAULT );
		Mat image = img1 - img2;
		//bitwise_and(img1,img2, image);//逻辑与，求交集
		//bitwise_or(img1,img2,image);//逻辑或，求并集
		//bitwise_not(img1,image);//逻辑非，求补集
		//bitwise_xor(img1,image,image);//异或，相同为0，相异为1
		for (int i = 0; i < image.rows; i++)
		{
			for (int j = 0; j < image.cols; j++)
			{
				int b = abs(img1.at<Vec3b>(i, j)[0] - img2.at<Vec3b>(i, j)[0]);
				int g = abs(img1.at<Vec3b>(i, j)[1] - img2.at<Vec3b>(i, j)[1]);
				int r = abs(img1.at<Vec3b>(i, j)[2] - img2.at<Vec3b>(i, j)[2]);
				
				int x = 150;
				if ((b) > x)
				{
					image.at<Vec3b>(i, j)[0] = img2.at<Vec3b>(i, j)[0];
					image.at<Vec3b>(i, j)[1] = img2.at<Vec3b>(i, j)[1];
					image.at<Vec3b>(i, j)[2] = img2.at<Vec3b>(i, j)[2];
					image.at<Vec3b>(i, j)[0] = b - img1.at<Vec3b>(i, j)[0];
					image.at<Vec3b>(i, j)[1] = g - img1.at<Vec3b>(i, j)[1];
					image.at<Vec3b>(i, j)[2] = r - img1.at<Vec3b>(i, j)[2];
					image.at<Vec3b>(i, j)[0] = x;
					image.at<Vec3b>(i, j)[1] = x;
					image.at<Vec3b>(i, j)[2] = x;
				}
				else
				{
					image.at<Vec3b>(i, j)[0] = (b);
					image.at<Vec3b>(i, j)[1] = (g);
					image.at<Vec3b>(i, j)[2] = (r);
				}
			}
		}
		//GaussianBlur( image, image, Size(5,5), 0, 0, BORDER_DEFAULT );
		//threshold(image, image, 10, 255, THRESH_BINARY);
		temp = QString::fromLatin1("E:/Test/%1_GlueShap.bmp").arg(n);
		imwrite(temp.toStdString().c_str(), image);
		qDebug() << temp.toStdString().c_str();
		continue;

		// 图像增强
		Mat imageGamma(image.size(), CV_32FC3);
		for (int i = 0; i < image.rows; i++)
		{
			for (int j = 0; j < image.cols; j++)
			{
				imageGamma.at<Vec3f>(i, j)[0] = log(1 + image.at<Vec3b>(i, j)[0]);
				imageGamma.at<Vec3f>(i, j)[1] = log(1 + image.at<Vec3b>(i, j)[1]);
				imageGamma.at<Vec3f>(i, j)[2] = log(1 + image.at<Vec3b>(i, j)[2]);

// 				imageGamma.at<Vec3f>(i, j)[0] = image.at<Vec3b>(i, j)[0];
// 				imageGamma.at<Vec3f>(i, j)[1] = image.at<Vec3b>(i, j)[1];
// 				imageGamma.at<Vec3f>(i, j)[2] = image.at<Vec3b>(i, j)[2];

			}
		}
		//归一化到0~255  
		normalize(imageGamma, imageGamma, 0, 255, CV_MINMAX);
		//转换成8bit图像显示  
		convertScaleAbs(imageGamma, imageGamma);

		//imshow("image2", imageGamma);
		temp = QString::fromLatin1("E:/Test/%1_GlueShap.bmp").arg(n);
		imwrite(temp.toStdString().c_str(), imageGamma);
		qDebug() << temp.toStdString().c_str();
	}
	
	waitKey();
	return 0;
#else
// 多张图直接执行

	vector<QString> vFilePath;
	QString filePathW;
	QString test = "E:/Test";
	QString testResult = "E:/Test_Result";
//	QString testResult = "E:/Test";
	TraverseDir(test, "GlueShap.bmp", vFilePath);
	qDebug() << "Traverse directory files done.";

	for (vector<QString>::iterator it = vFilePath.begin(); it != vFilePath.end(); ++it)
	{
		QString filePathR = *it;
		QString filePathW = filePathR;
		filePathW.replace(test, testResult);

		QString dirPath = filePathW.left(filePathW.lastIndexOf("/"));
		CreateDir(dirPath);

//		goodFeaturesToTrack_Demo(filePathR, filePathW);
// 		EdgeDetect_Sobel(filePathR, filePathW);
// 		EdgeDetect_Canny(filePathR, filePathW);
		EdgeDetect_Contour(filePathR, filePathW);
//		GrabCutImge(filePathR, filePathW);

// 		OC_Test_MV_3(filePathR, filePathW);
// 		OC_Test_MV_2(filePathR, filePathW);
//		OC_Test_MV_4(filePathR, filePathW);
//		Find_HW_X(filePathR, filePathW);
// 		Find_HW_Y(filePathR, filePathW);
// 		Find_HW_XY(filePathR, filePathW);
//		Find_HW_XY_2(filePathR, filePathW);
// 		OC_Test_HW_3(filePathR, filePathW);
// 		OC_Test_HW_4(filePathR, filePathW);
// 		OC_Test_HW_1(filePathR, filePathW);

// 		MophologyOpenClose(filePathR, filePathW);
// 		LineDetect_NoName(filePathR, filePathW);
// 		LineDetect_HT(filePathR, filePathW);
//		Mophology_WaterShed(filePathR, filePathW);

//		HarrisDectect_1(filePathR, filePathW);
//		FindCircle(filePathR, filePathW);
	}
//	system("pause");

	return 0;

#endif

	QApplication a(argc, argv);		
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

	QDlgMain dlg;
	dlg.show();
	a.exec();

	return 0;
}
