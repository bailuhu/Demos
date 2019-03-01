#pragma once
enum ALIGNTYPE
{ 
	ALIGN_NONE = 0, //不停靠 
	ALIGN_TOP, //停靠上边 	
	ALIGN_RIGHT, //停靠右边 
	ALIGN_LEFT, //停靠左边 
	ALIGN_BOTTOM
}; 

// CLessWindowDlg 对话框

class CLessWindowDlg : public CDialog
{
	DECLARE_DYNAMIC(CLessWindowDlg)

public:
	CLessWindowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLessWindowDlg();

// 对话框数据
	enum { IDD = IDD_LESSWINDOW };
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		void ShowAlphaWindow(ALIGNTYPE m_eAlignType);
		void OnClose();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	IDirectUI *m_pDirectUI; 	
	IUIFormObj* m_pMainBack1;
	int			m_nOp1;

	ALIGNTYPE m_eAlignType; //记录窗体停靠状态

	int nxFullScreen;
	int nyFullScreen;
};
