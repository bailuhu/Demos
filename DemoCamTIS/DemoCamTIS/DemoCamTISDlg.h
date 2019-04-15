// Open DeviceDlg.h : header file
//

#pragma once
#include "afxwin.h"


// COpenDeviceDlg dialog
class COpenDeviceDlg : public CDialog
{
// Construction
public:
	COpenDeviceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OPENDEVICE_DIALOG };

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

private:
	DShowLib::Grabber		grabber; // The instance of the Grabber class.

	CStatic m_cStaticVideoWindow; // This static field will be used for the video display.

};
