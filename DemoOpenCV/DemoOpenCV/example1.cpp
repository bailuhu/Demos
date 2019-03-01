#include "stdafx.h"

// load and show image
#if 0

#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

int main() 
{  
	Mat img = imread("D:\\4.jpg");  
	imshow("opencv demo", img);  
	waitKey(6000);
	return 0; 
}
#else

// #include "highgui.h"
//
// int main(int argc, char** argv)
// {
// 	IplImage* img = cvLoadImage("D:\\DemoOpenCV\\4.jpg");
// 	cvNamedWindow("Example", CV_WINDOW_AUTOSIZE);
// 	cvShowImage("Example", img);
// 	cvWaitKey(0);	// pause
// 	cvReleaseImage(&img);
// 	cvDestroyWindow("Example");
// 	return 0;
// }
#endif