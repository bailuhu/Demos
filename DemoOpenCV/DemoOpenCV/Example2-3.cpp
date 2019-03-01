#include "stdafx.h"

// #include <string>
// #include "highgui.hpp"
// #include "imgproc.hpp"
// using namespace std;
// using namespace cv;
// 
// int main(int argc, char** argv)
// {
// 	if (argv[1] == NULL) argv[1] = "D:\\Res\\tree.avi";
// 	
// 	char* wndName = "Example3";
// 	namedWindow(wndName, WINDOW_AUTOSIZE);
// 	VideoCapture cap;
// 	cap.open(string(argv[1]));
// 	Mat frame;
// 	while(1)
// 	{
// 		cap >> frame;
// 		if (!frame.data) break;
// 		imshow(wndName, frame);
// 		if (waitKey(33) >= 0) break;
// 	}
// 	return 0;
// }