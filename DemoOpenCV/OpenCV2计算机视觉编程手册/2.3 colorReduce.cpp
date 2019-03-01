#include "stdafx.h"
// 
// #include "..\DemoOpenCV\GlobalFunc.h"
// #include <cv.hpp>
// #include<highgui.hpp>
// #include <iostream>
// using namespace std;
// using namespace cv;
// 
// void colorReduce(Mat& img, int div=64)
// {
// 	int nl = img.rows;
// 	int nc = img.cols*img.channels();
// 	for (int j = 0; j < nl; j++)
// 	{
// 		// ptr函数可以得到图像任意行的首地址
// 		uchar* data = img.ptr<uchar>(j);
// 		for (int i = 0; i < nc; i++)
// 			data[i] = data[i]/div*div+div/2;
// 	}
// }
// 
// void colorReduceIterator(Mat& img, int div=64)
// {
// 	Mat_<Vec3b>::iterator it = img.begin<Vec3b>();
// 	Mat_<Vec3b>::iterator it_end = img.end<Vec3b>();
// 
// 	for (; it != it_end; ++it)
// 	{
// 		(*it)[0] = (*it)[0]/div*div+div/2;
// 		(*it)[1] = (*it)[1]/div*div+div/2;
// 		(*it)[2] = (*it)[2]/div*div+div/2;
// 	}
// }
// 
// void colorReduce(const Mat& imgIn, Mat& imgOut, int div=64)
// {
// 	imgOut.create(imgIn.rows, imgIn.cols, imgIn.type());
// 	int nl = imgIn.rows;
// 	int nc = imgIn.cols*imgIn.channels();
// 	for (int j = 0; j < nl; j++)
// 	{
// 		const uchar* data_in = imgIn.ptr<uchar>(j);
// 		// ptr函数可以得到图像任意行的首地址
// 		uchar* data_out = imgOut.ptr<uchar>(j);
// 		for (int i = 0; i < nc; i++)
// 			data_out[i] = data_in[i]/div*div+div/2;
// 	}
// }
// 
// void sharpen(const Mat& img, Mat& result)
// {
// 	result.create(img.size(), img.type());
// 	for (int j = 1; j < img.rows-1; j++)
// 	{
// 		const uchar* previous = img.ptr<const uchar>(j-1);
// 		const uchar* current = img.ptr<const uchar>(j);
// 		const uchar* next = img.ptr<const uchar>(j+1);
// 		uchar* output = result.ptr<uchar>(j);
// 		for (int i = 1; i < img.cols-1; i++)
// 		{
// 			*output=saturate_cast<uchar>(current[i]*5-current[i-1]-current[i+1]-previous[i]-next[i]);
// 			*output++;
// 		}
// 	}
// 	result.row(0).setTo(Scalar(0));
// 	result.row(result.rows-1).setTo(Scalar(0));
// 	result.col(0).setTo(Scalar(0));
// 	result.col(result.cols-1).setTo(Scalar(0));
// }
// 
// void sharpen2D(const Mat& img, Mat& result)
// {
// 	Mat kernel(3,3,CV_32F,Scalar(0));
// 	kernel.at<float>(0,0) = 0;
// 	kernel.at<float>(0,1) = -1.0;
// 	kernel.at<float>(0,2) = 0;
// 	kernel.at<float>(1,0) = -1.0;
// 	kernel.at<float>(1,1) = 5.0;
// 	kernel.at<float>(1,2) = -1.0;
// 	kernel.at<float>(2,0) = 0;
// 	kernel.at<float>(2,1) = -1.0;
// 	kernel.at<float>(2,2) = 0;
// 
// 
// 	filter2D(img, result, img.depth(), kernel);	// cv.hpp
// }
// 
// int main()
// {
// 	Mat img = imread(JPG4);
// 
// 	double duration = static_cast<double>(GetTickCount());
// 	Mat imgClone = img.clone();
// //	colorReduce(imgClone);
// 
// // 	Mat imgClone;
// // 	colorReduce(img, imgClone);
// 
// //	Mat imgClone;
// //	sharpen2D(img, imgClone);
// 
// 	addWeighted(img,0.9,img,0.9,0.9,imgClone);
// 
// 	duration = static_cast<double>(GetTickCount())-duration;
// 	duration /= getTickFrequency();	// ms
// 	cout << "Time spent: " << duration <<"ms" << endl;
// 
// 	namedWindow("image");
// 	imshow("image", img);
// 
// 	namedWindow("image-reduce");
// 	imshow("image-reduce", imgClone);
// 	waitKey(0);
// }