#pragma once

#include <algorithm>
#include <opencv/cv.h>

using namespace cv;


const int STORAGE_SIZE = 1 << 12;

#define hough_cmp_gt(l1,l2) (aux[l1] > aux[l2])

#define cvSobel_Core 5
#define overlap_check_brush 2

enum { XY_SHIFT = 16, XY_ONE = 1 << XY_SHIFT, DRAWING_STORAGE_BLOCK = (1<<12) - 256 };

static CV_IMPLEMENT_QSORT_EX( icvHoughSortDescent32s, int, hough_cmp_gt, const int* )

struct circle_found
{
	float score;	//匹配分数
	Vec3f circle;	//找到的圆
};

class FindShape
{
public:
	FindShape(void);
	~FindShape(void);

};

