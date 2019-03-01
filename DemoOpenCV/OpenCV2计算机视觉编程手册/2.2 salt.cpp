#include "stdafx.h"
// 
// #include<highgui.hpp>
// using namespace cv;
// 
// void salt(Mat& img, int n)
// {
// 	for (int k = 0; k < n; k++)
// 	{
// 		int i = rand()%img.cols;
// 		int j = rand()%img.rows;
// 		if (img.channels() == 1)	// gray image
// 			img.at<uchar>(j, i) = 255;
// 		else if (img.channels() == 3)// color image
// 		{
// 			img.at<Vec3b>(j,i)[0] = 255;
// 			img.at<Vec3b>(j,i)[1] = 255;
// 			img.at<Vec3b>(j,i)[2] = 255;
// 		}
// 	}
// }
// 
// int main()
// {
// 	Mat img = imread("D:\\Res\\lena.jpg");
// 	// 调用函数增加噪点
// 	salt(img, 3000);
// 	namedWindow("image");
// 	imshow("image", img);
// 	waitKey(0);
// }