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
// 	// ��ͼ����Ѱ�Ҿ��д�����ֵ�Ľǵ�
// 	cvGoodFeaturesToTrack(
// 						grayImage,	// ����ͼ��8λ������32λ����������ͨ�� 
// 						corners1,	// ��ʱ����32λͼ�񣬳ߴ�������ͼ��һ��
// 						corners2,	// ����һ����ʱͼ�񣬸�ʽ�ͳߴ���eig_imageһ�� 
// 						corners,	// �����������⵽�Ľǵ�
// 						&cornerCount,// �����������⵽�Ľǵ���
// 						0.3,		// �����С����ֵ�ĳ˷����ӡ�����ɽ���ͼ��ǵ����С��������
// 						5, 
// 						0,		// �������ӡ��õ��Ľǵ����С���롣ʹ��ŷ�Ͼ���
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