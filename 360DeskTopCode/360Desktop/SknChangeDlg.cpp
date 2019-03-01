// SknChangeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "360Desktop.h"
#include "SknChangeDlg.h"
#include <atlimage.h>
#include "360desktopdlg.h"
#define TIMER_ALPHA_SHOW 1007

// CSknChangeDlg 对话框

IMPLEMENT_DYNAMIC(CSknChangeDlg, CDialog)

CSknChangeDlg::CSknChangeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSknChangeDlg::IDD, pParent),
m_nOp(0),
m_eSkinType(ST_DEFAULT_SKIN)
{

}

CSknChangeDlg::~CSknChangeDlg()
{
	if (m_hBitmapSkinde)
	{
		DeleteObject(m_hBitmapSkinde);
		m_hBitmapSkinde = NULL;
	}
	if (m_hBitmapSkin1)
	{
		DeleteObject(m_hBitmapSkin1);
		m_hBitmapSkin1 = NULL;
	}
	if (m_hBitmapSkin2)
	{
		DeleteObject(m_hBitmapSkin2);
		m_hBitmapSkin2 = NULL;
	}
}

void CSknChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSknChangeDlg, CDialog)
	ON_WM_CREATE()
	ON_MESSAGE(DUILV_NOTIFY, OnLVNotify)
	ON_MESSAGE(DUISM_LBUTTONUP, OnSMLButtonUp)   //系统按钮的映射消息
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
// CSknChangeDlg 消息处理程序

int CSknChangeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	IDUITVItemGroup* pGroup = NULL;
	IDUITVItem* pItem = NULL;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	m_pDirectUI = (IDirectUI*)theApp.m_pDUIRes->CreateDirectUI(L"SknChange", HandleToLong(m_hWnd));
	m_pSknCloseBtn = (ICmdButton*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("SknCloseBtn"), TRUE);
	m_pSknCloseBtnRight = (ICmdButton*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("SknCloseBtnRight"), TRUE);
	m_pSknListview = (IDUIListView*)DIRECTUI_GETCONTROL(L"SknListview");

	CImage imageSkin;

	imageSkin.Load(_T("res\\skin.png"));
	m_hBitmapSkinde = imageSkin.Detach();
	imageSkin.Load(_T("res\\skin1.png"));
	m_hBitmapSkin1 = imageSkin.Detach();
	imageSkin.Load(_T("res\\skin2.png"));
	m_hBitmapSkin2 = imageSkin.Detach();

	pGroup = m_pSknListview->AddGroup(0, _T("更换皮肤"));

	IDUITVItem* pItem2 = NULL;
	pItem = pGroup->InsertItem(0, 0, _T("水墨风景"));
	pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmapSkin2);
	pItem = pGroup->InsertItem(0, 1, _T("优雅爵士"));
	pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmapSkin1);
	pItem2 = pGroup->InsertItem(0, 2, _T("默认主题"));
	pItem2->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmapSkinde);
	pGroup->SetExpand(TRUE);

	m_pSknListview->SetSelItem(pItem2, FALSE);

	m_pSknListview->RefreshView();

	m_pChangeSkn = (IUIFormObj*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("SknBg"), TRUE);
	m_iImageChangeSkn = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("SknBg2"), NULL, TRUE);
	m_iImageChangeSkn3 = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("SknBg3"), NULL, TRUE);
	m_iImageChangeSknde = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("SknBg"), NULL, TRUE);

	return 0;
}

LRESULT CSknChangeDlg::OnSMLButtonUp(WPARAM wParam,LPARAM lParam)
{
	IDUIControlBase* pControl = (IDUIControlBase*)wParam; 

	if (pControl == NULL)
	{
		return 0;
	}
	else if (pControl == m_pSknCloseBtn)
	{
		OnCancel();  //关闭当前窗口
	}
	else if (pControl == m_pSknCloseBtnRight)
	{
		OnCancel();  //关闭当前窗口
	}
	return 0;

}

LRESULT CSknChangeDlg::OnLVNotify(WPARAM wParam,LPARAM lParam)
{
	DUILVNotifyInfo* pInfo = (DUILVNotifyInfo*)wParam;
	if (pInfo)
	{
		switch(pInfo->eDUILVMsgId)
		{
		case DUI_LV_LBUTTONUP:
			{
				IDUIListView* pListView = (IDUIListView* )pInfo->lParam1;
				IDUITVItem* pItem = (IDUITVItem* )pInfo->lParam2;

				int nIndex = -1;
				if(pItem)
				{
					nIndex = pItem->GetIndex();
				}

				if (nIndex != -1)
				{
					int i = 0;
					if (nIndex == 0)
					{
						ChangeSkin(ST_DEFAULT_SKIN);//默认皮肤
					}
					else if (nIndex == 1)
					{
						ChangeSkin(ST_BLACK_SKIN);//优雅爵士
					}
					else if (nIndex == 2)
					{
						ChangeSkin(ST_GREEN_SKIN);//水墨风景
					}
				}
			}
			break;
		}
	}

	return TRUE;
}

void CSknChangeDlg::ChangeSkin(SKIN_TYPE eType)
{
	if(m_eSkinType == eType)return;
	m_eSkinType = eType;

	if (eType == ST_BLACK_SKIN)
	{
		//优雅爵士
		m_pChangeSkn->SetBackImageSec(m_iImageChangeSkn);
		((CMy360DesktopDlg*)theApp.m_pMainWnd)->ChangeSkin(ST_BLACK_SKIN);
	} 
	else if (eType == ST_GREEN_SKIN)
	{
		//水墨风景
		m_pChangeSkn->SetBackImageSec(m_iImageChangeSkn3);
		((CMy360DesktopDlg*)theApp.m_pMainWnd)->ChangeSkin(ST_GREEN_SKIN);
	}
	else if (eType == ST_DEFAULT_SKIN)
	{
		//默认皮肤
		m_pChangeSkn->SetBackImageSec(m_iImageChangeSknde);
		((CMy360DesktopDlg*)theApp.m_pMainWnd)->ChangeSkin(ST_DEFAULT_SKIN);
	} 

	CRect rect;
	GetWindowRect(&rect);
	rect.bottom -= 1;
	MoveWindow(&rect, false);
	rect.bottom += 1;
	MoveWindow(&rect, false);
}

void CSknChangeDlg::ShowAlphaWindow()
{
	if(m_nOp == 0)
	{
		CRect rectMenu;
		theApp.m_pMainWnd->GetWindowRect(&rectMenu);
		CRect rectSkin;
		GetWindowRect(&rectSkin);

		if (rectMenu.left < 390)
		{
			rectSkin.right = rectMenu.right + rectSkin.Width();
			rectSkin.left = rectMenu.right;
			rectSkin.bottom =(rectMenu.top + rectSkin.Height() + 71);
			rectSkin.top = (rectMenu.top + 71);

			MoveWindow(&rectSkin);

			m_pSknCloseBtn->SetVisible(FALSE, TRUE,FALSE);
			m_pSknCloseBtnRight->SetVisible(TRUE, TRUE,FALSE);
		

			SetTimer(TIMER_ALPHA_SHOW, 20, NULL);
			m_pDirectUI->SetOpacity(m_nOp);
			ShowWindow(SW_SHOW);
		}
		else
		{
			rectSkin.left = rectMenu.left - rectSkin.Width();
			rectSkin.right = rectMenu.left;
			rectSkin.bottom =(rectMenu.top + rectSkin.Height() + 71);
			rectSkin.top = (rectMenu.top + 71);

			MoveWindow(&rectSkin);

			m_pSknCloseBtnRight->SetVisible(FALSE, TRUE,FALSE);
			m_pSknCloseBtn->SetVisible(TRUE, TRUE,FALSE);

			SetTimer(TIMER_ALPHA_SHOW, 20, NULL);
			m_pDirectUI->SetOpacity(m_nOp);
			ShowWindow(SW_SHOW);
		}
	}
}

void CSknChangeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == TIMER_ALPHA_SHOW)
	{
		if (m_nOp > 255)
		{
			m_pDirectUI->SetOpacity(255);
			KillTimer(TIMER_ALPHA_SHOW);
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

void CSknChangeDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	if (GetForegroundWindow()->GetSafeHwnd() != ((CMy360DesktopDlg*)theApp.m_pMainWnd)->GetSafeHwnd())
	ShowWindow(SW_HIDE);
}
