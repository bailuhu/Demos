#include <QtWidgets/QApplication>

#include "QDlgMain.h"
#include "GlobalFunction.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;
#include "Huawei/dfspottestapi.h"

void help()
{
	cout << "\nThis program demonstrates line finding with the Hough transform.\n"
		"Usage:\n"
		"./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "E:/Test/18.bmp";
	Mat src = imread(filename, 0);
	if(src.empty())
	{
		help();
		cout << "can not open " << filename << endl;
		return -1;
	}
	Mat dst, cdst;
	Canny(src, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);
#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
	}
#endif
// 	imshow("source", src);
// 	imshow("detected lines", cdst);
// 	waitKey();

	const char* filenameW = argc >= 2 ? argv[1] : "F:/Test/18-HoughLinesP.bmp";
	imwrite(filenameW, cdst);
	return 0;
}


/*
/// Global variables
Mat src, src_gray;
Mat dst, detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
/// function CannyThreshold
/// brief Trackbar callback - Canny thresholds input with a ratio 1:3

void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur( src_gray, detected_edges, Size(3,3) );
	/// Canny detector
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	/// Using Canny’s output as a mask, we display our result
	dst = Scalar::all(0);
	src.copyTo( dst, detected_edges);
	imshow( window_name, dst );
}

int main( int argc, char** argv )
{
	/// Load an image
	src = imread( "E:/Test/18.bmp");
	if( !src.data )
	{ return -1; }
	/// Create a matrix of the same type and size as src (for dst)
	dst.create( src.size(), src.type() );
	/// Convert the image to grayscale
	cvtColor( src, src_gray, CV_BGR2GRAY );
	/// Create a window
	namedWindow( window_name, CV_WINDOW_NORMAL );
	/// Create a Trackbar for user to enter threshold
	createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
	/// Show the image
	CannyThreshold(0, 0);
	/// Wait until user exit program by pressing a key
	waitKey(0);
	return 0;
}
*/

void MV_OC_Test()
{
	// MV OC Test
	// 6
	int ocX = 0;
	int ocY = 0;
	int height = 0;
	int width = 0;
	int h = 800;
	int w = 1000;

	vector<QString> vFilePath;
	QString filePathW;
	TraverseDir("E:/Test", "bmp", vFilePath);
	qDebug() << "Traverse directory files done.";

	for (vector<QString>::iterator it = vFilePath.begin(); it != vFilePath.end(); ++it)
	{
		QString filePath = *it;
		Mat src = imread(filePath.toStdString());
		height = src.rows;
		width = src.cols;
		// Vertical
		Mat dst = Mat(src.clone(), Rect(width*1/3, 0, width*1/3, height));

		{
			cvtColor(dst, dst, CV_BGR2GRAY);
			vector<vector<Point> > contours;//contours的类型，双重的vector
			vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
			GaussianBlur(dst, dst, Size(5, 5), 0);
			filePathW = filePath;
			filePathW.replace(".bmp", "-blur31.bmp");
			filePathW.replace("E:/", "F:/");
			imwrite(filePathW.toStdString(), dst);
			threshold(dst, dst, 30, 255, THRESH_BINARY);

			qDebug() << filePathW.toStdString().c_str();
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
			Mat drawing = src.clone();  
			for (int i = 0; i < mc.size(); i++)
			{
				Scalar color = Scalar(255);
				drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
				double area = contourArea(contours[i]);
				if (area > 2000000)
				{
					Scalar color = Scalar(0,255,0);
					circle(drawing, mc[i], 4, color, -1, 8, 0);
					rectangle(drawing, Point(mc[i].x-w, mc[i].y-h), Point(mc[i].x+w, mc[i].y+h), color);
					ocX = mc[i].x;
					ocY = mc[i].y;
				}

			}
			;
			filePathW = filePath;
			filePathW.replace(".bmp", "-c.bmp");
			filePathW.replace("E:/", "F:/");
			imwrite(filePathW.toStdString(), drawing);
			qDebug() << filePathW.toStdString().c_str();
		}
		continue;
	}
}

void Huawei_OC_Test()
{
	// 6
	int ocX = 0;
	int ocY = 0;
	int height = 0;
	int width = 0;
	int h = 500;
	int w = 500;

	vector<QString> vFilePath;
	QString filePathW;
	TraverseDir("E:/Test", "bmp", vFilePath);

	for (vector<QString>::iterator it = vFilePath.begin(); it != vFilePath.end(); ++it)
	{
		QString filePath = *it;
		Mat src = imread(filePath.toStdString());
		// 2456 x 2054
		// 中心块 160x230 pixel
		height = src.rows;
		width = src.cols;
		{
			Mat dst;  
			cvtColor(src, dst, CV_BGR2GRAY);  
			vector<vector<Point> > contours;//contours的类型，双重的vector
			vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
			GaussianBlur(dst, dst, Size(5, 5), 0);  
			threshold(dst, dst, 20, 255, THRESH_BINARY);
			// 			filePathW = filePath;
			// 			filePathW.replace(".bmp", "-b.bmp");
			// 			filePathW.replace("E:/", "F:/");
			// 			imwrite(filePathW.toStdString(), dst);
			// 			qDebug() << filePathW.toStdString().c_str();
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
				double area = contourArea(contours[i]);
				if (area > 2000000)
				{
					Scalar color = Scalar(0,255,0);
					circle(src, mc[i], 4, color, -1, 8, 0);
					ocX = mc[i].x;
					ocY = mc[i].y;
				}

			}

			filePathW = filePath;
			filePathW.replace(".bmp", "-c.bmp");
			filePathW.replace("E:/", "F:/");
			imwrite(filePathW.toStdString(), src);
			qDebug() << filePathW.toStdString().c_str();
		}
		continue;

		{
			Mat src = imread(filePath.toStdString());
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
			filePathW = filePath;
			filePathW.replace(".bmp", "-e-d-b.bmp");
			filePathW.replace("E:/", "F:/");
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

			filePathW = filePath;
			filePathW.replace(".bmp", "-c.bmp");
			filePathW.replace("E:/", "F:/");
			imwrite(filePathW.toStdString(), src);
			qDebug() << filePathW.toStdString().c_str();
		}	
	}

}

void ClockWiseRotate()
{
	int width = 0;
	int height = 0;
	vector<QString> vFilePath;
	QString filePathW;
	TraverseDir("E:/Test", "bmp", vFilePath);
	qDebug() << "Traverse directory files done.";

	for (vector<QString>::iterator it = vFilePath.begin(); it != vFilePath.end(); ++it)
	{
		QString filePath = *it;
		Mat src = imread(filePath.toStdString());
		height = src.rows;
		width = src.cols;
		//flipCode: 翻转模式，
		// ==0 垂直翻转（沿X轴翻转）
		// > 0 水平翻转（沿Y轴翻转）
		// < 0 水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
		//clockwise 90
		transpose(src, src);
		flip(src, src, 1);
		//clockwise 180
		flip(src, src, -1);
		//clockwise 270
		transpose(src, src);
		flip(src, src, 0);
	}
}

bool HuaweiDllCall()
{
	char cTemp[MAX_PATH];
	sprintf_s(cTemp, "E:/xx/x/xNicolas/OpencvDemo/AA_Single/Huawei/Proj_IS.dll");

	//TraverseDir("E:/xx/x/x11", ".bmp");

	HMODULE hdll = LoadLibraryA(cTemp);
	if (hdll)
	{
		qDebug() << "LoadLibrary OK";
		//定义函数指针
		FARPROC fpFun = GetProcAddress(hdll, "dfSpotTestCreate");
		//执行dll接口函数
		if (!fpFun)
		{

		}
		
	}
	else
	{
		qDebug() << "LoadLibrary NG";
	//	DWORD dwErr = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), 0, (LPSTR)&lpMsgBuf, 0, NULL);

		char cTemp[MAX_PATH];
		sprintf_s(cTemp, "%s", lpMsgBuf);
		qDebug() << cTemp;
	}
	system("pause");
	return 0;
}

void MophologyOpenClose()
{
	Mat img = imread("E:/Test/18.bmp");
	namedWindow("原始图", WINDOW_NORMAL);
	imshow("原始图", img);
	//获取自定义核
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	Mat img1,img2;
// 	dilate(img, img1, element);//膨胀操作
// 	erode(img1, img2, element);//腐蚀操作
// 	namedWindow("腐蚀操作", WINDOW_NORMAL);
// 	imshow("腐蚀操作", img2);
	Mat out;
	morphologyEx(img, img1, MORPH_CLOSE, element);
	namedWindow("MORPH_CLOSE", WINDOW_NORMAL);
	imshow("MORPH_CLOSE", img1);

	morphologyEx(img, img2, MORPH_OPEN, element);
	namedWindow("MORPH_OPEN", WINDOW_NORMAL);
	imshow("MORPH_OPEN", img2);

	waitKey(0);
}


void EdgeDetect_Canny()
{

	//载入图像，转换为灰度图  
	QString filePath = "E:/Test/18.bmp";
	Mat src = imread(filePath.toStdString()); 
	Mat dst;
	//为canny边缘图像申请空间，1表示单通道灰度图
	cvtColor(src, dst, CV_BGR2GRAY);
	Canny( dst, dst, 20, 255, 3, true);//边缘检测
// 	namedWindow( "src", CV_WINDOW_AUTOSIZE);  
// 	namedWindow( "canny", CV_WINDOW_AUTOSIZE);  
// 	imshow( "src", src );  
// 	imshow( "canny", dst );

	QString filePathW = filePath;
	filePathW.replace(".bmp", "-canny.bmp");
	filePathW.replace("E:/", "F:/");
	imwrite(filePathW.toStdString(), dst);
	waitKey(0);

	

}


// int main(int argc, char *argv[])
// {
// 	MV_OC_Test();
// 	HuaweiDllCall();
// 	Huawei_OC_Test();
//	EdgeDetect_Canny();
//	MophologyOpenClose();
//	return 0;


// 	QApplication a(argc, argv);		
// 	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
// 
// 	QDlgMain dlg;
// 	dlg.show();
// 	a.exec();
// 
// 	return 0;






// 3
// 	for(int p = 1; p <= 4; p++)
// 	{
// 		for (int v = 1; v <= 4; v++)
// 		{
// 			for (int n = 0; n <= 30; n+=10)
// 			{
// 				char cTemp[MAX_PATH];
// 				sprintf_s(cTemp, "E:/xx/x/xp%dv%d.bmp", p, v);
// 				Mat src = imread(cTemp);
// 
// 				Mat dst;  
// 				cvtColor(src, dst, CV_BGR2GRAY);  
// 				vector<vector<Point> > contours;//contours的类型，双重的vector
// 				vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
// 				GaussianBlur(dst, dst, Size(5, 5), 0);  
// 				threshold(dst, dst, n, 255, THRESH_BINARY);
// 				findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));  
// 				/// 计算矩  
// 				vector<Moments> mu(contours.size());  
// 				for (int i = 0; i < contours.size(); i++)  
// 				{  
// 					mu[i] = moments(contours[i], false);  
// 				}  
// 				///  计算中心矩:  
// 				vector<Point2f> mc(contours.size());  
// 				for (int i = 0; i < contours.size(); i++)  
// 				{  
// 					mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
// 				}  
// 				/// 绘制轮廓  
// 				Mat drawing = Mat::zeros(dst.size(), CV_8UC1);  
// 				for (int i = 0; i < contours.size(); i++)  
// 				{  
// 					Scalar color = Scalar(255);  
// 					drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());  
// 					circle(src, mc[i], 4, color, -1, 8, 0);
// 					// 					Rect rect;
// 					// 					rect.x = mc[i].x-75;
// 					// 					rect.y = mc[i].y-100;
// 					// 					rect.width = 150;
// 					// 					rect.height = 200;
// 					// 					rectangle(drawing, rect, color, -1, 8, 0);
// 				}
// 				// namedWindow("outImage", WINDOW_AUTOSIZE);
// 				// imshow("outImage",drawing);
// 				sprintf_s(cTemp, "E:/xx/x/xp%dv%d-%d.bmp", p, v, n);
// 				imwrite(cTemp, src);
// 			}
// 		}
// 	}

	//3-1
// 	for(int p = 1; p <= 15; p++)
// 	{
// 		char cTemp[MAX_PATH];
// 		sprintf_s(cTemp, "E:/xx/x/x%d.bmp", p);
// 		Mat src = imread(cTemp);
// 
// 		Mat dst;  
// 		cvtColor(src, dst, CV_BGR2GRAY);  
// 		vector<vector<Point> > contours;//contours的类型，双重的vector
// 		vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据
// 		GaussianBlur(dst, dst, Size(5, 5), 0);  
// 		threshold(dst, dst, 10, 255, THRESH_BINARY);
// 		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
// 		/// 计算矩  
// 		vector<Moments> mu(contours.size());  
// 		for (int i = 0; i < contours.size(); i++)  
// 		{  
// 			mu[i] = moments(contours[i], false);  
// 		}  
// 		///  计算中心矩:  
// 		vector<Point2f> mc(contours.size());  
// 		for (int i = 0; i < contours.size(); i++)  
// 		{  
// 			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
// 		}  
// 		/// 绘制轮廓  
// 		Mat drawing = Mat::zeros(dst.size(), CV_8UC1);  
// 		for (int i = 0; i < contours.size(); i++)  
// 		{  
// 			Scalar color = Scalar(255);  
// 			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());  
// 			circle(src, mc[i], 4, color, -1, 8, 0);
// 		}
// 		sprintf_s(cTemp, "E:/xx/x/x%d-%d.bmp", p, 1);
// 		imwrite(cTemp, src);
// 		qDebug() << cTemp << endl;
// 	}
// 	return 0;


	

// 	return 0;
// }
