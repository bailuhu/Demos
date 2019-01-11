// Author: Nicolas Lee
// Date: 20180515
// Version:1.0.0.1

#ifndef __SEARCH_MARK__
#define __SEARCH_MARK__

#ifdef SEARCH_MARK_EXPORTS
#define SEARCH_MARK_API __declspec(dllexport)
#else
#define SEARCH_MARK_API __declspec(dllimport)
#endif

#include <vector>
using namespace std;

/************************************ Search Algrithm ****************************************/
// Search Mark Pos
namespace SearchMark
{
	typedef struct tagHotPixelStack
	{
		unsigned short x;
		unsigned short y;
		unsigned short Retpt;  //模拟EIP,周边8个点的位置编号
		/*检测周边8个点
		1 2 3
		4 0 5
		6 7 8  */
	}HotPixelStack;

	// size and pos
	typedef struct tagBlockInfo
	{
		double x;		//亮点在图像的横坐标 x
		double y;		//亮点在图像的横坐标 y
		double totalNum;//亮点大小
		double Value;   //亮点的亮度
		int l;
		int t;
		int r;
		int b;

	}BlockInfo;

	// 目标区域有多个BlockInfo，进行搜索，返回多个BlockInfo特征
	SEARCH_MARK_API bool GetTargetInfo(unsigned char* pSrc, int Src_W, int Src_H, int roiL, int roiT, int roiR, int RoiB, bool bBlack, int SegmentThres, vector<BlockInfo>& vBlockInfo);

	// 目标区域有单个BlockInfo，进行搜索，返回单个BlockInfo特征
	SEARCH_MARK_API bool GetTargetInfo(unsigned char* pSrc, int Src_W, int Src_H, int roiL, int roiT, int roiR, int RoiB, bool bBlack, int SegmentThres, BlockInfo& blockInfo);

}

#endif