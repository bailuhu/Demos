#include "stdafx.h"
// 
// #include <opencv.hpp>
// using namespace cv;
// 
// int main(int argc, char** argv)
// {
// 	if (argv[1] == NULL) argv[1] = "D:\\Res\\2.jpg";
// 	char* nameGray = "Example Gray";
// 	char* nameCanny = "Example Canny";
// 
// 	Mat img_rgb = imread(argv[1]);
// 	Mat img_gry, img_cny;
// 	cvtColor(img_rgb, img_gry, COLOR_BGR2GRAY);
// 
// 	namedWindow(nameGray, WINDOW_AUTOSIZE);
// 	namedWindow(nameCanny, WINDOW_AUTOSIZE);
// 	imshow(nameGray, img_gry);
// 	Canny(img_gry, img_cny, 10, 100, 3, true);
// 	imshow(nameCanny, img_cny);
// 	waitKey(0);
// 	return 0;
// }