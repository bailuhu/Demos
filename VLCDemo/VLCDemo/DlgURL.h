#pragma once
#include "afxwin.h"


// DlgURL dialog

class DlgURL : public CDialogEx
{
	DECLARE_DYNAMIC(DlgURL)

public:
	DlgURL(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgURL();

// Dialog Data
	enum { IDD = IDD_URL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editURL;
	TCHAR m_szUrl[MAX_PATH];
	
	afx_msg void OnBnClickedOk();
};
