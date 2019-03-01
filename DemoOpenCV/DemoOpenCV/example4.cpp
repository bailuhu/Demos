#include "stdafx.h"

// load and show processed image
/*
#include "cv.h"
#include "highgui.h"

void example2_4(IplImage* image)
{
	char* nameIn = "Example4-in";
	char* nameOut = "Example4-out";
	cvNamedWindow(nameIn);
	cvNamedWindow(nameOut);
	cvShowImage(nameIn, image);
	IplImage* out = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3);

	cvSmooth(image, out, CV_GAUSSIAN, 3, 3);
	cvShowImage(nameOut, out);
	cvReleaseImage(&out);
	cvWaitKey(0);
	cvDestroyWindow(nameIn);
	cvDestroyWindow(nameOut);
}

IplImage* doPyrDown(IplImage* in, int filter = CV_GAUSSIAN_5x5)
{
	int ratio = 2;
	assert(in->width%ratio == 0 && in->height%ratio == 0);
	IplImage* out = cvCreateImage(CvSize(in->width/ratio, in->height/ratio), in->depth, in->nChannels);
	cvPyrDown(in, out);
	cvRectangle(out, CvPoint(50,60), CvPoint(200,300), CvScalar(255,255,255));
	return out;
}

IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, int aperture)
{
	if (in->nChannels != 1)
		return 0;

	IplImage* out = cvCreateImage(cvSize(in->width, in->height), IPL_DEPTH_8U, 1);
	cvCanny(in, out, lowThresh, highThresh, aperture);
	return 0;
}

int main()
{
	IplImage* img = cvLoadImage("D:\\Res\\1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	example2_4(doPyrDown(img));
	example2_4(doCanny(img, 10, 100, 3));
}
*/