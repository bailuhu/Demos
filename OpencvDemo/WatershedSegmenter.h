#pragma once

#include <opencv/cv.h>

using namespace cv;

class WatershedSegmenter
{
public:
	WatershedSegmenter(void);
	~WatershedSegmenter(void);

	void setMarkers(const Mat& markerImg)
	{
		markerImg.convertTo(markers, CV_32S);
	}

	Mat process(const Mat& img)
	{
		watershed(img, markers);

		return markers;
	}

private:
	Mat markers;
};

