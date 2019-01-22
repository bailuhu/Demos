
#pragma once

#include <QDir>
#include <vector>

#include <opencv/cv.h>
#include <opencv/highgui.h>


#include "LineFinder.h"
#include "WatershedSegmenter.h"

#include "Chapter 08/harrisDetector.h"

using namespace std;
using namespace cv;

using namespace std;

void TraverseDir(QString dirPath, QString filter, vector<QString>& vFilePath);
bool CreateDir(QString dirPath);

// Image Process
void goodFeaturesToTrack_Demo(QString filePathR, QString filePathW);
int EdgeDetect_Sobel(QString filePathR, QString filePathW);
void EdgeDetect_Canny(QString filePathR, QString filePathW);
void EdgeDetect_Contour(QString filePathR, QString filePathW);

void GrabCutImge(QString filePathR, QString filePathW);

// 找到HW_X
Point Find_HW_X(QString filePathR, QString filePathW);
Point Find_HW_Y(QString filePathR, QString filePathW);
Point Find_HW_XY(QString filePathR, QString filePathW);
Point Find_HW_XY_2(QString filePathR, QString filePathW);
// 找到 MV OC X Y
Point OC_Test_MV_2(QString filePathR, QString filePathW);
// 修改版本，找HW OC
Point OC_Test_MV_4(QString filePathR, QString filePathW);

Point OC_Test_MV_3(QString filePathR, QString filePathW);

Point FindCircle(QString filePathR, QString filePathW);
bool SubStrateImg(QString filePathR1, QString filePathR2, QString filePathW);

void drawSquares( IplImage* img, CvSeq* squares ,const char* wndname);
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage ,int minarea, int maxarea, int minangle, int maxangle);