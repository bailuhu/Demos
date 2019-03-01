#include "stdafx.h"
// 
// #include <opencv.hpp>
// using namespace cv;
// 
// int main(int argc, char** argv)
// {
// 	if (argv[1] == NULL) argv[1] = "D:\\Res\\5.jpg";
// 	char* name1 = "Example1";
// 	char* name2 = "Example2";
// 
// 	Mat img = imread(argv[1]), img2;
// 	namedWindow(name1, WINDOW_AUTOSIZE);
// 	namedWindow(name2, WINDOW_AUTOSIZE);
// 	imshow(name1, img);
// 	pyrDown(img, img2);
// 	pyrDown(img2, img2);
// 	imshow(name2, img2);
// 	waitKey(0);
// 	return 0;
// }