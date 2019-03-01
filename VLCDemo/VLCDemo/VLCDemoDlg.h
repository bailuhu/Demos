
// VCLDemoDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "ExMFCButton.h"
#include "ExListBox.h"

class Player;

// CVCLDemoDlg dialog
class CVCLDemoDlg : public CDialogEx
{
// Construction
public:
	CVCLDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VCLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	ExListBox m_listFile;
	CStatic m_vedio;
	Player *m_pPlayer;
	ExMFCButton m_btnPlayPause;
	ExMFCButton m_btnStop;
	ExMFCButton m_btnURL;
	CSliderCtrl m_slider;
	CStatic m_time;
	ExMFCButton m_btnFile;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnLbnDblclkListFile();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnFile();

	afx_msg void OnBnClickedBtnUrl();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
