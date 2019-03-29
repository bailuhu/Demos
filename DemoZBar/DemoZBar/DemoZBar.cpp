// DemoZBar.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
using namespace std;
//ZBar
#include <D:\\ProgramFiles\\ZBar\\include\\zbar.h>
#pragma comment(lib, "D:\\ProgramFiles\\ZBar\\lib\\libzbar-0.lib")
using namespace zbar;

#include <opencv.hpp>
#include <highgui/highgui.hpp>
using namespace cv;


//zbar接口
string ZbarDecoder(Mat img)
{
	string result;
	ImageScanner scanner;
	const void *raw = (&img)->data;
	// configure the reader
	scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
	// wrap image data
	Image image(img.cols, img.rows, "Y800", raw, img.cols * img.rows);
	// scan the image for barcodes
	int n = scanner.scan(image);
	// extract results
	Image::SymbolIterator symbol = image.symbol_begin();
	for (; symbol != image.symbol_end(); ++symbol)
	{
		result = ("type: ");
		result.append(image.symbol_begin()->get_type_name());
		result.append(" code: ");
		result.append(image.symbol_begin()->get_data());
	}

	image.set_data(NULL, 0);
	return result;
}

//对二值图像进行识别，如果失败则开运算进行二次识别
string GetQRInBinImg(Mat binImg)
{
	string result = ZbarDecoder(binImg);
	if (result.empty())
	{
		Mat openImg;
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		morphologyEx(binImg, openImg, MORPH_OPEN, element);
		result = ZbarDecoder(openImg);
		imshow("openImg", openImg);
	}
	return result;
}

//main function
string GetQR(Mat img, Mat& binImg)
{
	//在otsu二值结果的基础上，不断增加阈值，用于识别模糊图像
	int thre = threshold(img, binImg, 0, 255, cv::THRESH_OTSU);
	string result;
	while (result.empty() && thre < 255)
	{
		threshold(img, binImg, thre, 255, cv::THRESH_BINARY);
		result = GetQRInBinImg(binImg);
		thre += 20;//阈值步长设为20，步长越大，识别率越低，速度越快
	}
	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{

	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

#if 0
	VideoCapture cap;
	cap.open(0);
	
	if (!cap.isOpened())//如果视频不能正常打开则返回
		return 0;
	Mat image;

	while (1)
	{
		cap >> image;//等价于cap.read(image);
		if (image.empty())//如果某帧为空则退出循环
			break;

		// 识别二维码
		if (!image.data)
		{
			cout << L"请确认图片" << endl;
			system("pause");
			return 0;
		}

		imwrite("C:\\Users\\simpl\\Desktop\\MouseWithoutBorders\\333.bmp", image);

		Mat imageGray;
		cvtColor(image, imageGray, CV_RGB2GRAY);
		Mat binImg;
		string result = GetQR(imageGray, binImg);
		cout << result << endl;
// 		int width = imageGray.cols;
// 		int height = imageGray.rows;
// 		uchar *raw = (uchar *)imageGray.data;
// 		Image imageZbar(width, height, "Y800", raw, width * height);
// 		scanner.scan(imageZbar); //扫描条码    
// 		Image::SymbolIterator symbol = imageZbar.symbol_begin();
// 		if (imageZbar.symbol_begin() == imageZbar.symbol_end())
// 		{
// 			//cout << L"查询条码失败，请检查图片！" << endl;
// 		}
// 		for (; symbol != imageZbar.symbol_end(); ++symbol)
// 		{
// 			cout << "type:" << symbol->get_type_name() << endl;
// 			cout << "code:" << symbol->get_data() << endl;
// 		//	system("pause");
// 		}
// 
		imshow("video", image);
		imshow("video binImg", binImg);
		waitKey(60);//每帧延时20毫秒
// 		imageZbar.set_data(NULL, 0);
	}
	cap.release();//释放资源
#else
	Mat image = imread("E:\\5_Workshop\\Other\\Demos\\DemoZBar\\Execute\\333.bmp");
	if (!image.data)
	{
		cout << "请确认图片" << endl;
		system("pause");
		return 0;
	}
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	Mat binImg;
	string result = GetQR(imageGray, binImg);
	cout << result << endl;
	waitKey(60);//每帧延时20毫秒

// 	int width = imageGray.cols;
// 	int height = imageGray.rows;
// 	uchar *raw = (uchar *)imageGray.data;
// 	Image imageZbar(width, height, "Y800", raw, width * height);
// 	scanner.scan(imageZbar); //扫描条码    
// 	Image::SymbolIterator symbol = imageZbar.symbol_begin();
// 	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
// 	{
// 		cout << "查询条码失败，请检查图片！" << endl;
// 	}
// 	for (; symbol != imageZbar.symbol_end(); ++symbol)
// 	{
// 		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
// 		cout << "条码：" << endl << symbol->get_data() << endl << endl;
// 	}
// 	imshow("Source Image", image);
// 	waitKey();
// 	imageZbar.set_data(NULL, 0);
#endif
	return 0;
}

