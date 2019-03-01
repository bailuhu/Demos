// SetMenuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "360Desktop.h"
#include "360DesktopDlg.h"
#include "SetMenuDlg.h"

#define TIMER_ALPHA 1006

// CSetMenuDlg 对话框

IMPLEMENT_DYNAMIC(CSetMenuDlg, CDialog)

CSetMenuDlg::CSetMenuDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSetMenuDlg::IDD, pParent),
m_pMainBack(NULL),
m_nOp(0)
{

}

CSetMenuDlg::~CSetMenuDlg()
{
}

void CSetMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSetMenuDlg, CDialog)
	ON_WM_CREATE()    //获取主体窗口映射消息
	ON_MESSAGE(DUISM_LBUTTONUP, OnSMLButtonUp)   //系统按钮的映射消息
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSetMenuDlg 消息处理程序

int CSetMenuDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct)  == -1)
	{
		return -1;
	}
	m_pDirectUI = (IDirectUI*)theApp.m_pDUIRes->CreateDirectUI(L"SettingMenu", HandleToLong(m_hWnd));
	m_pMainBack = (IUIFormObj*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("BackBg"), TRUE);
	m_pSetMenuBg =  (IUIFormObj*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("BackBg"), TRUE);
	m_pChangeSkin = (ICmdButton*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ChangeSkin"), TRUE);

	m_dSknChangeWin.Create(CSknChangeDlg::IDD, NULL);

	return 0;
}


LRESULT CSetMenuDlg::OnSMLButtonUp(WPARAM wParam,LPARAM lParam)
{
	IDUIControlBase* pControl = (IDUIControlBase*)wParam; 

	if (pControl == NULL)
	{
		return 0;
	}
	else if (pControl == m_pChangeSkin)
	{
		m_dSknChangeWin.ShowAlphaWindow();
	}
	return 0;
}

void CSetMenuDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);
	// TODO: 在此处添加消息处理程序代码
	ShowWindow(SW_HIDE);
}

void CSetMenuDlg::ChangeSkin(SKIN_TYPE eType)
{
	IDUIImageBase* pImage = NULL;
	if (eType == ST_BLACK_SKIN)
	{
		pImage = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("SettingMenuBg2"), NULL, TRUE);
	} 
	else if (eType == ST_GREEN_SKIN)
	{
		pImage = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("SettingMenuBg3"), NULL, TRUE);
	}
	else if (eType == ST_DEFAULT_SKIN)
	{
		pImage = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("SettingMenuBg1"), NULL, TRUE);
	}

	if (pImage)
	{
		m_pMainBack->SetBackImageSec(pImage);
		RECT rect;
		GetWindowRect(&rect);
		rect.bottom -= 1;
		MoveWindow(&rect, false);
		rect.bottom += 1;
		MoveWindow(&rect, true);
	}
}

void CSetMenuDlg::ShowAlphaWindow()
{
	if(m_nOp == 0)
	{
		CRect rect;
		theApp.m_pMainWnd->GetWindowRect(&rect);
		CRect rectMenu;
		int nTempTop = 0;
		int nTempBottom = 0;
		GetWindowRect(&rectMenu);
		
		if (rect.left < 273)
		{
			rectMenu.right = rect.right + rectMenu.Width();
			rectMenu.left = rect.right ;
			rectMenu.bottom =rect.top + rectMenu.Height() + 3;
			rectMenu.top = rect.top + 3;

			MoveWindow(&rectMenu);

			SetTimer(TIMER_ALPHA, 20, NULL);
			m_pDirectUI->SetOpacity(m_nOp);
			ShowWindow(SW_SHOW);
		}
		else
		{
			rectMenu.left = rect.left - rectMenu.Width();
			rectMenu.right = rect.left;
			rectMenu.bottom =rect.top + rectMenu.Height() + 3;
			rectMenu.top =  rect.top + 3;

			MoveWindow(&rectMenu);

			SetTimer(TIMER_ALPHA, 20, NULL);
			m_pDirectUI->SetOpacity(m_nOp);
			ShowWindow(SW_SHOW);
		}
	}
}

void CSetMenuDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == TIMER_ALPHA)
	{
		if (m_nOp  > 255)
		{
			m_pDirectUI->SetOpacity(255);
			KillTimer(TIMER_ALPHA);
			m_nOp = 0;
		}
		else
		{
			m_nOp += 20;
			m_pDirectUI->SetOpacity(m_nOp);
		}
	}
	CDialog::OnTimer(nIDEvent);
}
