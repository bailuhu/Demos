// LessWindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "360Desktop.h"
#include "LessWindowDlg.h"


// CLessWindowDlg 对话框

IMPLEMENT_DYNAMIC(CLessWindowDlg, CDialog)

CLessWindowDlg::CLessWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLessWindowDlg::IDD, pParent)
{
	nxFullScreen = GetSystemMetrics(SM_CXSCREEN);
	nyFullScreen = GetSystemMetrics(SM_CYSCREEN);
}

CLessWindowDlg::~CLessWindowDlg()
{
}

void CLessWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLessWindowDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CLessWindowDlg 消息处理程序

int CLessWindowDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct)  == -1)
	{
		return -1;
	}

	m_pDirectUI = (IDirectUI*)theApp.m_pDUIRes->CreateDirectUI(L"LessWindow", HandleToLong(m_hWnd));
	m_pMainBack1 = (IUIFormObj*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("LessWindowBg"), TRUE);
	
	return 0;
}

void CLessWindowDlg::ShowAlphaWindow(ALIGNTYPE m_eAlignType)
{
	if (m_eAlignType == ALIGN_TOP)
	{
		CRect rect(0, 0, nxFullScreen, 40);
		ShowWindow(SW_HIDE);
		MoveWindow(&rect);
		ShowWindow(SW_SHOW);
	}
	else if (m_eAlignType == ALIGN_BOTTOM)
	{
		CRect rect(0,(nyFullScreen - 40), nxFullScreen, nyFullScreen);
		ShowWindow(SW_HIDE);
		MoveWindow(&rect);
		ShowWindow(SW_SHOW);
	}
	else if (m_eAlignType == ALIGN_LEFT)
	{
		CRect rect(0, 0, 40, nyFullScreen);
		ShowWindow(SW_HIDE);
		MoveWindow(&rect);
		ShowWindow(SW_SHOW);
	}
	else if (m_eAlignType == ALIGN_RIGHT)
	{
		CRect rect((nxFullScreen - 40), 0, nxFullScreen, nyFullScreen);
		ShowWindow(SW_HIDE);
		MoveWindow(&rect);
		ShowWindow(SW_SHOW);
	}
}

void CLessWindowDlg::OnClose()
{
	OnCancel();
}