#include "stdafx.h"
// 
// #include <opencv.hpp>
// using namespace cv;
// 
// void example2_5(Mat& image)
// {
// 	char* nameIn = "Example2_5-in";
// 	namedWindow(nameIn, WINDOW_AUTOSIZE);
// 	imshow(nameIn, image);
// 
// 	
// 	Mat outGaussian;
// 	char* nameOut = "GaussianBlur";
// 	namedWindow(nameOut, WINDOW_AUTOSIZE);
// 	GaussianBlur(image, outGaussian, Size(3,3), 3, 3);
// 	imshow(nameOut, outGaussian);
// 
// 	Mat outBlur;
// 	char* nameBlur = "Blur";
// 	namedWindow(nameBlur, WINDOW_AUTOSIZE);
// 	blur(image, outBlur, Size(3,3));
// 	imshow(nameBlur, outBlur);
// 
// 	Mat outMedian;
// 	char* nameMedian = "MedianBlur";
// 	namedWindow(nameMedian, WINDOW_AUTOSIZE);
// 	medianBlur(image, outMedian, 5);
// 	imshow(nameMedian, outMedian);
// 
// 	// Ë«±ßÂË²¨
// 	Mat outBilateralFilter;
// 	char* nameBilateralFilter = "BilateralFilter";
// 	namedWindow(nameBilateralFilter, WINDOW_AUTOSIZE);
// 	bilateralFilter(image, outBilateralFilter, 3, 0.3, 0.3);
// 	imshow(nameBilateralFilter, outBilateralFilter);
// 
// 	waitKey(0);
// }
// 
// int main(int argc, char** argv)
// {
// 	if (argv[1] == NULL) argv[1] = "D:\\Res\\2.jpg";
// 
// 	Mat img = imread(argv[1]);
// 	example2_5(img);
// }