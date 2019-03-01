
// DemoCVDlg.h : ͷ�ļ�
//

#pragma once

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

#define IMAGE_CHANNELS 3

// CDemoCVDlg �Ի���
class CDemoCVDlg : public CDialogEx
{
// ����
public:
	CDemoCVDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DEMOCV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
