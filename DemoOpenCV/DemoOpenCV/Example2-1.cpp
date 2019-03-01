#include "stdafx.h"

// A simple OpenCV program that loads an image from disk and display it on the screen
//#include <opencv.hpp>
// #include <iostream>
// #include "highgui/highgui.hpp"	// for faster compile, we use only the needed header file, not generic opencv.hpp
// using namespace cv;	// Employing the "using namespace" directive
// using namespace std;
// 
// int main(int argc, char** argv)
// {
// 	if (argv[1] == NULL) argv[1] = "d:\\Res\\2.jpg";
// 
// 	Mat img = imread(argv[1], -1);
// 	if (img.empty()) return -1;
// 	char* wndName = "Example1";
// 	//namedWindow(wndName, WINDOW_AUTOSIZE);
// 	namedWindow(wndName, WINDOW_NORMAL);
// 	imshow(wndName, img);
// 	int keyNum = waitKey(0);
// 	cout << keyNum << endl;
// 	destroyWindow(wndName);
// 	return 0;
// }