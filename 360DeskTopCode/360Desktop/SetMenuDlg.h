#pragma once
#include "360DesktopDlg.h"
#include "SknChangeDlg.h"
// CSetMenuDlg �Ի���

class CSetMenuDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetMenuDlg)

public:
	CSetMenuDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetMenuDlg();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSMLButtonUp(WPARAM wParam,LPARAM lParam);
	static LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// �Ի�������
	enum { IDD = IDD_SETTINGMENU };
	IUIFormObj* m_pSetMenuBg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	IDirectUI *m_pDirectUI; 

	ICmdButton *m_pChangeSkin;
		
	CSknChangeDlg m_dSknChangeWin;
	IUIFormObj* m_pMainBack;
	int			m_nOp;

public:
	CSknChangeDlg*	GetChangeWinDlg(){ return &m_dSknChangeWin; }
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	void	ChangeSkin(SKIN_TYPE eType);
	void	ShowAlphaWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
