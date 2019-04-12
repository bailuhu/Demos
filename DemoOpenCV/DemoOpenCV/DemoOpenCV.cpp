// TestOpenCV.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include "opencv2/highgui/highgui.hpp"

int _tmain(char agrc, char** agrv)
{
	cv::Mat src = cv::imread("C:\\Users\\simpl\\Desktop\\Temp\\3 generation 20 pcs\\1.bmp", -1);
	if (src.empty())
	{
		std::cout << "read image failed." << std::endl;
	}
	cv::namedWindow("src", cv::WINDOW_NORMAL);
	cv::imshow("src", src);
	cv::waitKey(0);
	cv::destroyWindow("src");
	return 0;
}
