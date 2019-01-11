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

	//加载源图并转换为灰度图
	src1 = imread("E:/Test/2.bmp");
	cvtColor( src1, src1_gray, CV_BGR2GRAY );
	//创建窗口
	namedWindow( source_window1, CV_WINDOW_AUTOSIZE );
	//创建滑块条，调节允许的角点个数
	createTrackbar( "BlockSize:", source_window1, &blockSize, maxTrackbar, goodFeaturesToTrack_Demo );
	createTrackbar( "MaxCorner:", source_window1, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo );
	goodFeaturesToTrack_Demo( 0, 0 );
	waitKey(0);
	return(0);

	vector<QString> vFilePath;
	QString filePathW;
	QString test = "E:/Test";
	QString testResult = "E:/Test_Result";
//	QString testResult = "E:/Test";
	TraverseDir(test, "bmp", vFilePath);
	qDebug() << "Traverse directory files done.";

	for (vector<QString>::iterator it = vFilePath.begin(); it != vFilePath.end(); ++it)
	{
		QString filePathR = *it;
		QString filePathW = filePathR;
		filePathW.replace(test, testResult);

		QString dirPath = filePathW.left(filePathW.lastIndexOf("/"));
		CreateDir(dirPath);

		goodFeaturesToTrack_Demo(filePathR, filePathW);
// 		EdgeDetect_Sobel(filePathR, filePathW);
// 		EdgeDetect_Canny(filePathR, filePathW);

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


	QApplication a(argc, argv);		
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

	QDlgMain dlg;
	dlg.show();
	a.exec();

	return 0;
}
