#include "stdafx.h"

// #include <opencv.hpp>
// #include <iostream>
// using namespace std;
// using namespace cv;
// 
// int main(int argc, char** argv)
// {
// 	if (argv[1] == NULL) argv[1] = "D:\\Res\\tree.avi";
// 	if (argv[2] == NULL) argv[2] = "D:\\Res\\Copy.avi";
// 	char* nameEx = "Example2_10";
// 	char* nameLP = "Log_Polar";
// 
// 	namedWindow(nameEx, WINDOW_AUTOSIZE);
// 	namedWindow(nameLP, WINDOW_AUTOSIZE);
// 	VideoCapture capture;
// 	capture.open(argv[1]);
// 	double fps = capture.get(CAP_PROP_FPS);
// 	Size size((int)capture.get(CAP_PROP_FRAME_WIDTH), (int)capture.get(CAP_PROP_FRAME_HEIGHT));
// 	VideoWriter writer;
// 	writer.open("D:\\Res\\Copy.avi", CV_FOURCC('M','J','P','G'), fps, size);
// 	Mat logPolar_frame(size, CV_8UC3), bgr_frame;
// 	for (;;)
// 	{
// 		capture >> bgr_frame;
// 		if (bgr_frame.empty()) break;
// 		imshow("Example2_10",bgr_frame);
// 		logPolar(bgr_frame, logPolar_frame, Point2f(bgr_frame.cols/2, bgr_frame.rows/2), 40, WARP_FILL_OUTLIERS);
// 		imshow("Log_Polar", logPolar_frame);
// 		writer <<logPolar_frame;
// 		if (waitKey(10) == 27) break;
// 	}
// 	capture.release();
// 	return 0;
// }