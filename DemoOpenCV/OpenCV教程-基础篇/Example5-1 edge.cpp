#include "stdafx.h"

/*程序名：edge.c*/
// #include "cv.h"
// #include "highgui.h"
// 
// char wndName[] = "Edge";
// char barName[] = "Threshold";
// int edgeThresh = 1;
// 
// IplImage* img = 0, *cEdge = 0, *gray = 0, *edge = 0;
// 
// // 定义跟踪条的callback函数
// void on_trackbar(int h)
// {
// 	cvSmooth(gray, edge, CV_BLUR, 3, 3, 0);
// 	cvNot(gray, edge);
// 
// 	// 对灰度图像进行边缘检测
// 	cvCanny(gray, edge, (float)edgeThresh, (float)edgeThresh*3, 3);
// 	cvZero(cEdge);
// 
// 	// copy edge points
// 	cvCopy(img, cEdge, edge);
// 	// show image
// 	cvShowImage(wndName, cEdge);
// }
// 
// int main(int argc, char** argv)
// {
// 	char* filename = argc == 2?argv[1]:(char*)"D:\\Res\\fruits.jpg";
// 	if ((img = cvLoadImage(filename, 1)) == 0)
// 		return -1;
// 
// 	// create output image
// 	cEdge = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
// 	// convert color image to gray
// 	gray = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
// 	edge = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
// 	cvCvtColor(img, gray, CV_BGR2GRAY);
// 
// 	// create a window
// 	cvNamedWindow(wndName, 1);
// 	// create a toolbar
// 	cvCreateTrackbar(barName, wndName, &edgeThresh, 100, on_trackbar);
// 	on_trackbar(0);
// 	cvWaitKey(0);
// 	cvReleaseImage(&img);
// 	cvReleaseImage(&gray);
// 	cvReleaseImage(&edge);
// 	cvDestroyWindow(wndName);
// 	return 0;
// }