// TestOpenCV.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include "opencv.hpp"

int _tmain(char agrc, char** agrv)
{
	cv::Mat src = cv::imread("C:\\Users\\simpl\\Desktop\\Temp\\3 generation 20 pcs\\src.png", -1);
	cv::Mat dst;
	if (src.empty())
	{
		std::cout << "read image failed." << std::endl;
	}
	cv::logPolar(
		src, // Input color frame
		dst, // Output log-polar frame
		cv::Point2f( // Centerpoint for log-polar transformation
		src.cols / 2, // x
		src.rows / 2 // y
		),
		40, // Magnitude (scale parameter)
		cv::WARP_FILL_OUTLIERS // Fill outliers with 'zero'
		);
// 	cv::pyrDown(src, src);
// 	cv::Canny(src, src, 10, 100, 3, true);
	cv::namedWindow("src", cv::WINDOW_NORMAL);
	cv::imshow("src", src);
	cv::namedWindow("dst", cv::WINDOW_NORMAL);
	cv::imshow("dst", dst);
	cv::waitKey(0);
	cv::destroyWindow("src");
	cv::destroyWindow("dst");
	return 0;
}
