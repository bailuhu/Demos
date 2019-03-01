// 360DesktopDlg.h : ͷ�ļ�
//

#pragma once
#include "SetMenuDlg.h"
#include "SknChangeDlg.h"
#include "LessWindowDlg.h"
#define NEAR_SIDE 3 //�������غ�����Ļ�ϱ��������أ���ʹ�����Դ���

// CMy360DesktopDlg �Ի���
class CMy360DesktopDlg : public CDialog
{
	// ����
public:
	CMy360DesktopDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMy360DesktopDlg();

	// �Ի�������
	enum { IDD = IDD_MY360DESKTOP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnDUILButtonUp(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnTabSelChanged(WPARAM wParam,LPARAM lParam);

	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg	LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()


private:
	// ����DirectUI����
	IDirectUI *m_pDirectUI;
	IUIFormObj* m_pMainBack;

	//ϵͳ��ť:��С������󻯡��ر�
	ICmdButton *m_pBtnMin;
	ICmdButton *m_pBtnMax;
	ICmdButton *m_pBtnClose;
	//���ܲ˵���ť
	ICmdButton *m_pBtnMenu;

	IDUITabCtrl	*m_pTabCtrl;

	IDUIListView *m_pListView;
	IDUIListView *m_pListViewNew;
	IDUIListView *m_pHotAppListView;
	IDUIListView *m_pListViewLately;
	
	HBITMAP		m_hBitmap0;
	HBITMAP		m_hBitmap1;
	HBITMAP		m_hBitmap2;
	HBITMAP		m_hBitmap3;
	HBITMAP		m_hBitmap4;

	IUIFormObj *m_pMyApp;
	IUIFormObj *m_pHotApp;
	IUIFormObj *m_pLatelyApp;

	IDUIStatic *m_pBtnNum1;
	IDUIStatic *m_pBtnNum2;
	IDUIStatic *m_pBtnNum3;
	IDUIStatic *m_pBtnNum4;

	CSetMenuDlg m_dSetMenu;

	BOOL m_bStartTimer;
	

	int			m_nSel;

	IDUIListView* m_pLeftForm;
	IDUIListView* m_pRightForm;

	CSknChangeDlg m_dSknChange;

	ALIGNTYPE m_eAlignType; //��¼����ͣ��״̬ 
	int		m_nScreenX,m_nScreenY;
	BOOL	m_bHasHide;
	BOOL	m_bPress;
	int m_nFullScreenX,m_nFullScreenY;
	CLessWindowDlg m_dLessWindow;
	CPoint	m_ptOld;

public:
	void	HideSide(BOOL hide);
	void	ChangeSkin(SKIN_TYPE eType);
	void	SetSel(int nSel);
	void	InitForm(int nDirection);

};
