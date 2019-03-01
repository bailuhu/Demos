 #include "stdafx.h"

// load and show video with trackbar

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
// 	cvNamedWindow("example3", CV_WINDOW_AUTOSIZE);
// 	g_capture = cvCreateFileCapture("D:\\DemoOpenCV\\tree.avi");
// 
// 	int frames = (int)cvGetCaptureProperty(g_capture, CV_CAP_PROP_FRAME_COUNT);
// 	if (frames != 0)
// 	{
// 		cvCreateTrackbar("position", "example3", &g_slider_pos, frames, onTrackbarSlide);
// 	}
// 	IplImage* frame;
// 
// 	while(1)
// 	{
// 		frame = cvQueryFrame(g_capture);
// 		if (!frame)
// 			break;
// 
// 		cvShowImage("Example3", frame);
// 		char c = cvWaitKey(33);
// 		if (c == 27) 
// 			break;
// 	}
// 	cvReleaseImage(&frame);
// 	cvDestroyWindow("Example3");
// 	return 0;
// }