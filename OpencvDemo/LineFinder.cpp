#include "LineFinder.h"


LineFinder::LineFinder(void):deltaRho(1),deltaTheta(CV_PI/180),minVote(10),minLength(0),maxGap(0)
{

}


LineFinder::~LineFinder(void)
{
}

void LineFinder::setAccResolution(double dRho, double dTheta)
{
	deltaRho = dRho;
	deltaTheta = dTheta;
}

void LineFinder::setMinVote(int minv)
{
	minVote = minv;
}

void LineFinder::setLineLengthAndGap(double len, double gap)
{
	minLength = len;
	maxGap = gap;
}

vector<Vec4i> LineFinder::findLines(Mat& binary)
{
	lines.clear();
	HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
	return lines;
}

void LineFinder::drawDetectLines(Mat& img, Scalar color /*= Scalar(255,0,0)*/)
{
	vector<Vec4i>::const_iterator it = lines.begin();
	while (it != lines.end())
	{
		double theta = (*it)[1];
		double arcLent = CV_PI/180.0;
		if(theta > CV_PI/4 || theta < CV_PI/4*3)
		{
			Point pt1((*it)[0],(*it)[1]);
			Point pt2((*it)[2],(*it)[3]);
			line(img, pt1, pt2, color);
		}

		++it;
	}
}