#include "stdafx.h"
// 
// #include "highgui.hpp"
// #include "imgproc.hpp"
// #include <iostream>
// #include <fstream>
// 
// using namespace std;
// using namespace cv;
// 
// int g_slider_pos = 0;
// int g_run = 1, g_dontset = 0; // start out int single step mode
// VideoCapture g_cap;
// 
// void OnTrackbarSlide(int pos, void*)
// {
// 	g_cap.set(CAP_PROP_POS_FRAMES, pos);
// 	if (!g_dontset)
// 		g_run = 1;
// 	g_dontset = 0;
// }
// 
// int main(int argc, char** argv)
// {
// 	if (argv[1] == NULL) argv[1] = "D:\\Res\\tree.avi";
// 	char* wndName = "Example2_4";
// 
// 	namedWindow(wndName, WINDOW_AUTOSIZE);
// 	g_cap.open(string(argv[1]));
// 	int frames	= (int) g_cap.get(CAP_PROP_FRAME_COUNT);
// 	int tmpW	= (int) g_cap.get(CAP_PROP_FRAME_WIDTH);
// 	int tmpH	= (int) g_cap.get(CAP_PROP_FRAME_HEIGHT);
// 	cout <<"Video has " <<frames << " frames of dimensions("<< tmpW <<", " << tmpH << ")," << endl;
// 	createTrackbar("Position", wndName, &g_slider_pos, frames, OnTrackbarSlide);
// 	Mat frame;
// 	while(1)
// 	{
// 		if (g_run != 0)
// 		{
// 			g_cap >> frame; if (!frame.data) break;
// 			int current_pos = (int) g_cap.get(CAP_PROP_POS_FRAMES);
// 			g_dontset = 1;
// 			setTrackbarPos("Position", wndName, current_pos);
// 			imshow(wndName, frame);
// 			g_run -= 1;
// 		}
// 		char c = (char) waitKey(30);
// 		if (c == 's') // single step
// 		{
// 			g_run = 1; 
// 			cout << "Single step, run = " << g_run << endl;
// 		}
// 		if (c == 'r')
// 		{
// 			g_run = -1;
// 			cout << "Run mode, run = " << g_run << endl;
// 		}
// 		if (c == 27)
// 			break;
// 	}
// 	return 0;
// }