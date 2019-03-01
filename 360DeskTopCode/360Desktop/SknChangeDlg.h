#pragma once
enum SKIN_TYPE
{
	ST_DEFAULT_SKIN = 0,
	ST_BLACK_SKIN = 1,
	ST_GREEN_SKIN = 2
};
// CSknChangeDlg 对话框

class CSknChangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSknChangeDlg)

public:
	CSknChangeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSknChangeDlg();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSMLButtonUp(WPARAM wParam,LPARAM lParam);
	afx_msg	LRESULT OnLVNotify(WPARAM wParam,LPARAM lParam);

	// 对话框数据
	enum { IDD = IDD_SKNCHANGE };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	IDirectUI *m_pDirectUI;

	IDUIListView *m_pSknListview;
	IUIFormObj *m_pMainBack;
	IDUIImageBase *m_iImageMainBack;
	IUIFormObj *m_pSetingMenu;
	IDUIImageBase *m_iImageSetingMenu;
	IUIFormObj *m_pChangeSkn;
	IDUIImageBase *m_iImageChangeSkn;
	IDUIImageBase *m_iImageChangeSkn3;
	IDUIImageBase *m_iImageChangeSknde;

	HBITMAP	m_hBitmapSkinde;
	HBITMAP	m_hBitmapSkin1;
	HBITMAP	m_hBitmapSkin2;
	int	m_nOp;
	SKIN_TYPE	m_eSkinType;

	DECLARE_MESSAGE_MAP()
public:
	void ChangeSkin(SKIN_TYPE eType);
	void ShowAlphaWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	ICmdButton *m_pSknCloseBtn;
	ICmdButton *m_pSknCloseBtnRight;
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
