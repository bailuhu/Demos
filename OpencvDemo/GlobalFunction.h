
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

// �ҵ�HW_X
Point Find_HW_X(QString filePathR, QString filePathW);
Point Find_HW_Y(QString filePathR, QString filePathW);
Point Find_HW_XY(QString filePathR, QString filePathW);
Point Find_HW_XY_2(QString filePathR, QString filePathW);
// �ҵ� MV OC X Y
Point OC_Test_MV_2(QString filePathR, QString filePathW);
// �޸İ汾����HW OC
Point OC_Test_MV_4(QString filePathR, QString filePathW);

Point OC_Test_MV_3(QString filePathR, QString filePathW);