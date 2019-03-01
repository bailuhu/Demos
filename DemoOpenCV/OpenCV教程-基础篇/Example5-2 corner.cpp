#include "stdafx.h"

// #include <stdio.h>
// #include "cv.h"
// #include "highgui.h"
// #define max_corner 100
// 
// int main(int argc, char** argv)
// {
// 	int cornerCount = max_corner;
// 	CvPoint2D32f corners[max_corner];
// 	IplImage* srcImage = 0, *grayImage = 0, *corners1 = 0, *corners2 = 0;
// 	int i;
// 	CvScalar color = CV_RGB(255,0,0);
// 	char* fileName = argc == 2?argv[1]:(char*)"D:\\Res\\pic3.png";
// 
// 	cvNamedWindow("image", 1);
// 	srcImage = cvLoadImage(fileName, 1);
// 	grayImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);
// 	
// 	cvCvtColor(srcImage, grayImage, CV_BGR2GRAY);
// 	corners1 = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_32F, 1);
// 	corners2 = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_32F, 1);
// 
// 	// 在图像中寻找具有大特征值的角点
// 	cvGoodFeaturesToTrack(
// 						grayImage,	// 输入图像，8位整数或32位浮点数，单通道 
// 						corners1,	// 临时浮点32位图像，尺寸与输入图像一致
// 						corners2,	// 另外一个临时图像，格式和尺寸与eig_image一致 
// 						corners,	// 输出参数，检测到的角点
// 						&cornerCount,// 输出参数，检测到的角点数
// 						0.3,		// 最大、最小特征值的乘法因子。定义可接受图像角点的最小质量因子
// 						5, 
// 						0,		// 限制因子。得到的角点的最小距离。使用欧氏距离
// 						3, // block size
// 						0, // not use harris
// 						0.4);
// 	printf("num corners found: %d\n", cornerCount);
// 	// draw circles at each corner location in the gray image and
// 	// print out a list the corners
// 	if (cornerCount > 0)
// 	{
// 		for (i = 0; i < cornerCount; i++)
// 		{
// 			cvCircle(srcImage, CvPoint((int)(corners[i].x),(int)(corners[i].y)), 6, color, 2, CV_AA, 0);
// 		}
// 	}
// 
// 	cvShowImage("image", srcImage);
// 	cvReleaseImage(&srcImage);
// 	cvReleaseImage(&grayImage);
// 	cvReleaseImage(&corners1);
// 	cvReleaseImage(&corners2);
// 	cvWaitKey(0);
// 	return 0;
// }