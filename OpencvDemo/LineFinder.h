#pragma once

#include <opencv/cv.h>

using namespace cv;

class LineFinder
{
public:
	LineFinder(void);
	~LineFinder(void);

	void setAccResolution(double dRho, double dTheta);
	void setMinVote(int minv);
	void setLineLengthAndGap(double len, double gap);
	vector<Vec4i> findLines(Mat& binary);
	void drawDetectLines(Mat& img, Scalar color = Scalar(255,0,0));
private:
	Mat img;
	vector<Vec4i> lines;
	double deltaRho;
	double deltaTheta;
	int minVote;
	double minLength;
	double maxGap;


};

