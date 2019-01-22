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
		unsigned short Retpt;  //ģ��EIP,�ܱ�8�����λ�ñ��
		/*����ܱ�8����
		1 2 3
		4 0 5
		6 7 8  */
	}HotPixelStack;

	// size and pos
	typedef struct tagBlockInfo
	{
		double x;		//������ͼ��ĺ����� x
		double y;		//������ͼ��ĺ����� y
		double totalNum;//�����С
		double Value;   //���������
		int l;
		int t;
		int r;
		int b;

	}BlockInfo;

	// Ŀ�������ж��BlockInfo���������������ض��BlockInfo����
	SEARCH_MARK_API bool GetTargetInfo(unsigned char* pSrc, int Src_W, int Src_H, int roiL, int roiT, int roiR, int RoiB, bool bBlack, int SegmentThres, vector<BlockInfo>& vBlockInfo);

	// Ŀ�������е���BlockInfo���������������ص���BlockInfo����
	SEARCH_MARK_API bool GetTargetInfo(unsigned char* pSrc, int Src_W, int Src_H, int roiL, int roiT, int roiR, int RoiB, bool bBlack, int SegmentThres, BlockInfo& blockInfo);

}

#endif