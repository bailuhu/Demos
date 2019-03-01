#include "stdafx.h"

// load and show video
// #include "highgui.h"
// #include "cv.h"
// 
// int g_slider_pos = 0;
// CvCapture* g_capture = NULL;
//
// void onTrackbarSlide(int pos)
// {
// 	cvSetCaptureProperty(g_capture, CV_CAP_PROP_POS_FRAMES, pos);
// }
//
// int main()
// {
// 	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
// 	CvCapture* capture = cvCreateFileCapture("D:\\DemoOpenCV\\tree.avi");
// 	IplImage* frame;
// 	while(1)
// 	{
// 		frame = cvQueryFrame(capture);
// 		if (!frame)
// 			break;
// 
// 		cvShowImage("Example2", frame);
// 		char c = cvWaitKey(33);
// 		if (c == 27) 
// 			break;
// 	}
// 	cvReleaseImage(&frame);
// 	cvDestroyWindow("Example2");
// }