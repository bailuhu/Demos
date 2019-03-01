#include "stdafx.h"
// 
// #include <cv.h>
// #include <highgui.h>
// 
// int main(int argc, char** argv)
// {
// 	
// 	char* fileName = argc == 2?argv[1]:(char*)"D:\\Res\\fruits.jpg";
// 	IplImage* src = cvLoadImage(fileName);
// 	IplImage* dst = cvCreateImage(cvSize(256,256), 8, 3);
// 	IplImage* src2 = cvCreateImage(cvGetSize(src), 8, 3);
// 	cvLogPolar(src,dst, CvPoint2D32f(src->width/2, src->height/2), 40, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
// 	cvLogPolar(dst,src2, CvPoint2D32f(src->width/2, src->height/2), 40, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP);
// 	char* wndName = "log-polar";
// 	char* wndName2 = "inverse log-polar";
// 	cvNamedWindow(wndName, 1);
// 	cvShowImage(wndName, dst);
// 	cvNamedWindow(wndName2, 1);
// 	cvShowImage(wndName2, src2);
// 	cvWaitKey();
// 	return 0;
// }