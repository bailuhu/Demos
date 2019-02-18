// DemoZBar.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
using namespace std;
//ZBar
#include <D:\\Program\\ZBar\\include\\zbar.h>
#pragma comment(lib, "D:\\Program\\ZBar\\lib\\libzbar-0.lib")
using namespace zbar;

#include <opencv.hpp>
#include <highgui.hpp>
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{

	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

#if 1
	VideoCapture cap;
	cap.open(0);
	
	if (!cap.isOpened())//如果视频不能正常打开则返回
		return 0;
	Mat frame;
	while (1)
	{
		cap >> frame;//等价于cap.read(frame);
		if (frame.empty())//如果某帧为空则退出循环
			break;

		// 识别二维码
		if (!frame.data)
		{
			cout << L"请确认图片" << endl;
			system("pause");
			return 0;
		}
		Mat imageGray;
		cvtColor(frame, imageGray, CV_RGB2GRAY);
		int width = imageGray.cols;
		int height = imageGray.rows;
		uchar *raw = (uchar *)imageGray.data;
		Image imageZbar(width, height, "Y800", raw, width * height);
		scanner.scan(imageZbar); //扫描条码    
		Image::SymbolIterator symbol = imageZbar.symbol_begin();
		if (imageZbar.symbol_begin() == imageZbar.symbol_end())
		{
			//cout << L"查询条码失败，请检查图片！" << endl;
		}
		for (; symbol != imageZbar.symbol_end(); ++symbol)
		{
			cout << "type:" << endl << symbol->get_type_name() << endl << endl;
			cout << "code:" << endl << symbol->get_data() << endl << endl;
			system("pause");
		}

		imshow("video", frame);
		waitKey(60);//每帧延时20毫秒
		imageZbar.set_data(NULL, 0);
	}
	cap.release();//释放资源
#else
	Mat image = imread("E:/5 Workshop/DemoZBar/Execute/csdn.png");
	if (!image.data)
	{
		cout << "请确认图片" << endl;
		system("pause");
		return 0;
	}
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //扫描条码    
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "查询条码失败，请检查图片！" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
		cout << "条码：" << endl << symbol->get_data() << endl << endl;
	}
	imshow("Source Image", image);
	waitKey();
	imageZbar.set_data(NULL, 0);
#endif
	return 0;
}

