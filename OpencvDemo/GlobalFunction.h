
#pragma once

#include <QDir>
#include <vector>

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

using namespace std;

void TraverseDir(QString dirPath, QString filter, vector<QString>& vFilePath);
bool CreateDir(QString dirPath);

// Image Process
int EdgeDetect_Sobel(QString filePathR, QString filePathW);
void EdgeDetect_Canny(QString filePathR, QString filePathW);

// ’“µΩHW_X
void Find_HW_X(QString filePathR, QString filePathW);
void Find_HW_Y(QString filePathR, QString filePathW);
void Find_HW_XY(QString filePathR, QString filePathW);
// ’“µΩ MV OC X Y
cv::Point OC_Test_MV_2(QString filePathR, QString filePathW);


cv::Point OC_Test_MV_3(QString filePathR, QString filePathW);