#include "stdafx.h"
// 
// #include "highgui.h"
// #include "math.h"
// int main(int argc, char** argv)
// {
// 	
// 	char* fileName = argc == 2?argv[1]:(char*)"D:\\Res\\fruits.jpg";
// 	IplImage* src = cvLoadImage(fileName);
// 	IplImage* dst = cvCloneImage(src);
// 	int delta = 1;
// 	int angle = 0;
// 	int opt = 1;	// 1: 旋转+缩放 0：旋转
// 
// 	double factor;
// 	cvNamedWindow("src", 1);
// 	cvShowImage("src", src);
// 	for (;;)
// 	{
// 		float m[6];
// 		CvMat mat = cvMat(2, 3, CV_32F, m);
// 		int w = src->width;
// 		int h = src->height;
// 		if (opt)
// 			factor = (cos(angle*CV_PI/180.0)+1.05)*2;
// 		else
// 			factor = 1;
// 		m[0] = float(factor*cos(-angle*2*CV_PI/180.0));
// 		m[1] = float(factor*sin(-angle*2*CV_PI/180.0));
// 		m[3] = -m[1];
// 		m[4] = m[0];
// 		// 将旋转中心移至图像中间
// 		m[2] = w*0.5f;
// 		m[5] = h*0.5f;
// 		// dst(x,y) = A*src(x,y)+b
// 		cvGetQuadrangleSubPix(src, dst, &mat);
// 		cvNamedWindow("dst", 1);
// 		cvShowImage("dst", dst);
// 		if (cvWaitKey(5) == 27)
// 			break;
// 		angle = int(angle+delta)%360;
// 	}// for-loop
// 	return 0;
// }