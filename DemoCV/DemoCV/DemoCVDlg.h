
// DemoCVDlg.h : 头文件
//

#pragma once

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

#define IMAGE_CHANNELS 3

// CDemoCVDlg 对话框
class CDemoCVDlg : public CDialogEx
{
// 构造
public:
	CDemoCVDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DEMOCV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadimg();
	void ShowImage( IplImage* img, UINT ID );
	int m_width;
	int m_height;
	CRect m_rect;
	IplImage* m_image;
	afx_msg void OnBnClickedEdgedetect();
	void ResizeImage(IplImage* img);
};
