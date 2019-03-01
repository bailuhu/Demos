// 360DesktopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "360Desktop.h"
#include "360DesktopDlg.h"
#include <atlimage.h>
#include <Windows.h>
#include "SknChangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RIGHT_TIMER 1000
#define LEFT_TIMER 2000

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CMy360DesktopDlg 对话框

CMy360DesktopDlg::CMy360DesktopDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMy360DesktopDlg::IDD, pParent),
m_pListView(NULL),
m_hBitmap0(NULL),
m_hBitmap1(NULL),
m_hBitmap2(NULL),
m_hBitmap3(NULL),
m_hBitmap4(NULL),
m_pHotApp(NULL),
m_pMyApp(NULL),
m_pMainBack(NULL),
m_bStartTimer(FALSE),
m_nSel(1),
m_pLeftForm(NULL),
m_pRightForm(NULL),
m_eAlignType(ALIGN_NONE),
m_bHasHide(FALSE),
m_bPress(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nScreenX = GetSystemMetrics(SM_CXSCREEN);
	m_nScreenY = GetSystemMetrics(SM_CYSCREEN);
	m_nFullScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
	m_nFullScreenY = GetSystemMetrics(SM_CYFULLSCREEN);
}

CMy360DesktopDlg::~CMy360DesktopDlg()
{
	if(m_hBitmap0)
	{
		DeleteObject(m_hBitmap0);
		m_hBitmap0 = NULL;
	}
	if(m_hBitmap1)
	{
		DeleteObject(m_hBitmap1);
		m_hBitmap1 = NULL;
	}
	if(m_hBitmap2)
	{
		DeleteObject(m_hBitmap2);
		m_hBitmap2 = NULL;
	}
	if(m_hBitmap3)
	{
		DeleteObject(m_hBitmap3);
		m_hBitmap3 = NULL;
	}
	if(m_hBitmap4)
	{
		DeleteObject(m_hBitmap4);
		m_hBitmap4 = NULL;
	}
}

void CMy360DesktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy360DesktopDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()    //获取主体窗口映射消息
	ON_MESSAGE(DUISM_LBUTTONUP, OnDUILButtonUp)   //系统按钮的映射消息
	ON_MESSAGE(DUI_TABMSG_SELCHANGED,OnTabSelChanged)
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMy360DesktopDlg 消息处理程序

BOOL CMy360DesktopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy360DesktopDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CMy360DesktopDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CMy360DesktopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CMy360DesktopDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	long nGroupid = 0;
	int nNum = 0;

	IDUITVItemGroup* pGroup = NULL;
	IDUITVItem* pItem = NULL;


	//主体窗口
	m_pDirectUI = (IDirectUI*)theApp.m_pDUIRes->CreateDirectUI(_T("main"), (long)HandleToLong(GetSafeHwnd()));
	m_pMainBack = (IUIFormObj*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("Background"), TRUE);

	m_pDirectUI->SetDispatchMsg(WM_LBUTTONDOWN);
	m_pDirectUI->SetDispatchMsg(WM_LBUTTONUP);

	//系统按钮：最小化、最大化、关闭
	m_pBtnMin = (ICmdButton*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ButtonMin"), TRUE);
	m_pBtnMax = (ICmdButton*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ButtonMax"), TRUE);
	m_pBtnClose = (ICmdButton*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ButtonClose"), TRUE);
	m_pBtnNum1 = (IDUIStatic*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ButtonNum1"), TRUE);
	m_pBtnNum2 = (IDUIStatic*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ButtonNum2"), TRUE);
	m_pBtnNum3 = (IDUIStatic*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ButtonNum3"), TRUE);
	m_pBtnNum4 = (IDUIStatic*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ButtonNum4"), TRUE);

	//功能菜单按钮
	m_pBtnMenu = (ICmdButton*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("ButtonMenu"), TRUE);
	//Tab 控件
	m_pTabCtrl = (IDUITabCtrl*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("MyCtrl"), TRUE);
	ASSERT(m_pTabCtrl);

	m_pTabCtrl->SetSelectedItem(0);

	m_dSetMenu.Create(CSetMenuDlg::IDD,NULL);
	m_dLessWindow.Create(CLessWindowDlg::IDD, NULL);

	m_pMyApp = (IUIFormObj*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("MyApp"), TRUE);
	m_pHotApp = (IUIFormObj*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("HotApp"), TRUE);
	m_pLatelyApp = (IUIFormObj*)m_pDirectUI->GetObjectByCaption(DUIOBJTYPE_PLUGIN, _T("LatelyApp"), TRUE);

	//默认为 我的应用
	m_pListView = (IDUIListView*)DIRECTUI_GETCONTROL(L"ListView");
	m_pHotAppListView = (IDUIListView*)DIRECTUI_GETCONTROL(L"HotAppListView");
	m_pListViewNew = (IDUIListView*)DIRECTUI_GETCONTROL(L"ListViewNew");
	m_pListViewLately = (IDUIListView*)DIRECTUI_GETCONTROL(L"LatelyUsed");

	//加载列表图标
	CImage image;
	image.Load(_T("res\\icon0.png"));
	m_hBitmap0 = image.Detach();
	image.Load(_T("res\\icon1.png"));
	m_hBitmap1 = image.Detach();
	image.Load(_T("res\\icon2.png"));
	m_hBitmap2 = image.Detach();
	image.Load(_T("res\\icon3.png"));
	m_hBitmap3 = image.Detach();
	image.Load(_T("res\\icon4.png"));
	m_hBitmap4 = image.Detach();

	//我的应用列表1
	m_pListView->InsertItem(0, L"我的QQ", FALSE);	
	m_pListView->SetItemGraphic(0, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap0);
	m_pListView->InsertItem(1, L"我的电脑", FALSE);	
	m_pListView->SetItemGraphic(1, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap1);
	m_pListView->InsertItem(2, L"热门游戏", FALSE);	
	m_pListView->SetItemGraphic(2, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap2);
	m_pListView->InsertItem(3, L"我的文档", FALSE);	
	m_pListView->SetItemGraphic(3, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap3);
	m_pListView->InsertItem(4, L"常用软件", FALSE);	
	m_pListView->SetItemGraphic(4, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap4);
	m_pListView->InsertItem(5, L"添加更多", FALSE);	
	m_pListView->RefreshView();

	//我的应用列表2
	m_pListViewNew->InsertItem(0, L"我的QQ", FALSE);	
	m_pListViewNew->SetItemGraphic(0, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap0);
	m_pListViewNew->InsertItem(1, L"我的电脑", FALSE);	
	m_pListViewNew->SetItemGraphic(1, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap1);
	m_pListViewNew->InsertItem(2, L"热门游戏", FALSE);	
	m_pListViewNew->SetItemGraphic(2, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap2);
	m_pListViewNew->InsertItem(3, L"添加更多", FALSE);	
	m_pListViewNew->RefreshView();

	//今日热点列表 分组
	int nLoop = 0;
	pGroup = m_pHotAppListView->AddGroup(1000, _T("应用列表"));
	pGroup->SetExpand(TRUE);
	pItem = pGroup->InsertItem(0, 0, _T("常用软件"));	
	pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap4);
	pItem = pGroup->InsertItem(1, 1, _T("我的QQ"));		
	pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap0);
	pItem = pGroup->InsertItem(2, 2, _T("我的电脑"));
	pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap1);
	pItem = pGroup->InsertItem(3, 3, _T("热门游戏"));
	pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap2);
	pItem = pGroup->InsertItem(4, 4, _T("我的文档"));
	pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap3);

	pGroup = m_pHotAppListView->AddGroup(2000, _T("今日热点"));
	pGroup->SetExpand(TRUE);
	for (nLoop = 0; nLoop < 10; ++nLoop)
	{
		int nPos = nLoop % 5;
		if (nPos == 0) 
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("常用软件"));	
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap4);
		}
		else if (nPos == 1) 
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("我的QQ"));		
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap0);
		}
		else if (nPos == 2)
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("我的电脑"));
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap1);
		}
		else if (nPos == 3)
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("热门游戏"));
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap2);
		}
		else if (nPos == 4)
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("我的文档"));
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap3);
		}
	}

	pGroup = m_pHotAppListView->AddGroup(3000, _T("常用桌面"));
	pGroup->SetExpand(TRUE);
	for (nLoop = 0; nLoop < 10; ++nLoop)
	{
		int nPos = nLoop % 5;
		if (nPos == 0) 
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("常用软件"));	
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap4);
		}
		else if (nPos == 1) 
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("我的QQ"));		
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap0);
		}
		else if (nPos == 2)
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("我的电脑"));
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap1);
		}
		else if (nPos == 3)
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("热门游戏"));
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap2);
		}
		else if (nPos == 4)
		{
			pItem = pGroup->InsertItem(nLoop, nLoop, _T("我的文档"));
			pItem->SetGraphics(DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap3);
		}
	}
	m_pHotAppListView->ResizeThis();
	m_pHotAppListView->RefreshView();
	//最近使用列表
	m_pListViewLately->InsertItem(0, L"我的QQ", FALSE);	
	m_pListViewLately->SetItemGraphic(0, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap0);
	m_pListViewLately->InsertItem(1, L"我的电脑", FALSE);	
	m_pListViewLately->SetItemGraphic(1, DUILVI_STATE_NORMAL, (OLE_HANDLE)m_hBitmap1);
	m_pListViewLately->RefreshView();

	SetWindowText(_T("360 Desktop"));

	SetSel(m_nSel);
	return 0;
}

LRESULT CMy360DesktopDlg::OnDUILButtonUp(WPARAM wparam, LPARAM lparam)
{
	IDUIControlBase* pControl = (IDUIControlBase*)wparam; 

	LONG nHorzLeft = 0;
	LONG nHorzLeftNew = 0;

	if(pControl == NULL)
	{ 
		return 0; 
	} 

	else if (pControl == m_pBtnMin)
	{
		ShowWindow(SW_MINIMIZE);   //最小化按钮
	}

	else if (pControl == m_pBtnMax)
	{
		//最大化按钮，在360桌面程序中是实现全屏
	}

	else if (pControl == m_pBtnClose)
	{
		OnCancel();//关闭按钮
	}

	else if (pControl == m_pBtnMenu)
	{
		m_dSetMenu.ShowAlphaWindow();
	}
	else if (pControl == m_pBtnNum1)
	{
		if(!m_bStartTimer)
		{   
			if (m_nSel != 1 && m_bStartTimer == FALSE)
			{
				InitForm(LEFT_TIMER);
				SetTimer(LEFT_TIMER, 20, NULL);
			}
			SetSel(1);
		}
	}
	else if (pControl == m_pBtnNum2)
	{
		if(!m_bStartTimer)
		{
			if (m_nSel != 2 && m_bStartTimer == FALSE)
			{
				if(m_nSel < 2)
				{
					InitForm(RIGHT_TIMER);
					SetTimer(RIGHT_TIMER, 20, NULL	);
				}
				else if (m_nSel > 2)
				{
					InitForm(LEFT_TIMER);
					SetTimer(LEFT_TIMER, 20, NULL	);
				}
			}
			SetSel(2);
		}
	}
	else if (pControl == m_pBtnNum3)
	{
		if(!m_bStartTimer)
		{
			if (m_nSel != 3 && m_bStartTimer == FALSE)
			{
				if(m_nSel < 3)
				{
					InitForm(RIGHT_TIMER);
					SetTimer(RIGHT_TIMER, 20, NULL	);
				}
				else if (m_nSel > 3 )
				{
					InitForm(LEFT_TIMER);
					SetTimer(LEFT_TIMER, 20, NULL	);
				}
			}
			SetSel(3);
		}
	}
	else if (pControl == m_pBtnNum4)
	{
		if(!m_bStartTimer)
		{
			if (m_nSel != 4 && m_bStartTimer == FALSE)
			{
				InitForm(RIGHT_TIMER);
				SetTimer(RIGHT_TIMER, 20, NULL	);	
			}
			SetSel(4);
		}
	}

	POINT pt;
	GetCursorPos(&pt);
	CRect rectMenu;
	theApp.m_pMainWnd->GetWindowRect(&rectMenu);
	CRect rc;
	GetWindowRect(&rc);
	CRect rcSkn;
	GetWindowRect(&rcSkn);
	
	rcSkn.right = rectMenu.left;
	rcSkn.left = rectMenu.left - 389;
	rcSkn.top = rectMenu.top + 71;
	rcSkn.bottom = rectMenu.top + 71 + 435;

	rc.right = rectMenu.right;
	rc.left = rectMenu.left;
	rc.top = rectMenu.top;
	rc.bottom = rectMenu.top +38;

	if (!rc.PtInRect(pt) && !rcSkn.PtInRect(pt))
	{
		m_dSetMenu.GetChangeWinDlg()->ShowWindow(SW_HIDE);
	}
	return 0;
}

LRESULT CMy360DesktopDlg::OnTabSelChanged(WPARAM wparam,LPARAM lparam)
{
	switch(wparam)
	{
	case 0:
		{	
			if(m_pHotApp && m_pMyApp && m_pLatelyApp)
			{
				m_pHotApp->SetVisible(FALSE, TRUE,FALSE);
				m_pLatelyApp->SetVisible(FALSE, TRUE,FALSE);
				m_pMyApp->SetVisible(TRUE, TRUE, FALSE);
			}
			break;
		}
	case 1:
		{
			if(m_pHotApp && m_pMyApp && m_pLatelyApp)
			{
				m_pMyApp->SetVisible(FALSE, TRUE, FALSE);
				m_pLatelyApp->SetVisible(FALSE, TRUE,FALSE);
				m_pHotApp->SetVisible(TRUE, TRUE,FALSE);
			}
			break;
		}
	case 2:
		{	
			if(m_pHotApp && m_pMyApp && m_pLatelyApp)
			{
				m_pHotApp->SetVisible(FALSE, TRUE,FALSE);
				m_pMyApp->SetVisible(FALSE, TRUE,FALSE);
				m_pLatelyApp->SetVisible(TRUE, TRUE, FALSE);
			}
			break;
		}
	}
	return 0;
}

void CMy360DesktopDlg::InitForm(int nDirection)
{
	IDUIPos* pPos = NULL;
	DUIPosition pos;
	IDUIPos* pPosNew = NULL;
	DUIPosition posnew;
	m_bStartTimer = TRUE;

	pPos = (IDUIPos*)m_pListView->GetPosition();
	pPosNew = (IDUIPos*)m_pListViewNew->GetPosition();

	pos = pPos->GetPosStruct();
	posnew = pPosNew->GetPosStruct();

	if (nDirection == RIGHT_TIMER)
	{
		if (pos.horzPos.nLeft_Top == 0)
		{
			m_pLeftForm = m_pListView;
			m_pRightForm = m_pListViewNew;

			posnew.horzPos.nLeft_Top = 300;
			pPosNew->SetPosStruct(posnew);
			m_pListViewNew->SetPosition(pPosNew);
		}
		else
		{
			m_pLeftForm = m_pListViewNew;
			m_pRightForm = m_pListView;

			pos.horzPos.nLeft_Top = 300;
			pPos->SetPosStruct(pos);
			m_pListView->SetPosition(pPos);
		}
	}
	else if (nDirection == LEFT_TIMER)
	{
		if (pos.horzPos.nLeft_Top == 0)
		{
			m_pLeftForm = m_pListViewNew;
			m_pRightForm	= m_pListView;

			posnew.horzPos.nLeft_Top = -300;
			pPosNew->SetPosStruct(posnew);
			m_pListViewNew->SetPosition(pPosNew);
		}
		else
		{
			m_pLeftForm = m_pListView;
			m_pRightForm = m_pListViewNew;

			pos.horzPos.nLeft_Top = -300;
			pPos->SetPosStruct(pos);
			m_pLeftForm->SetPosition(pPos);
		}
	}
}

void CMy360DesktopDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nIDEvent == RIGHT_TIMER)
	{
		IDUIPos* pPos = NULL;
		DUIPosition pos;
		IDUIPos* pPosNew = NULL;
		DUIPosition posnew;

		pPos = (IDUIPos*)m_pLeftForm->GetPosition();
		pPosNew = (IDUIPos*)m_pRightForm->GetPosition();

		pos = pPos->GetPosStruct();
		posnew = pPosNew->GetPosStruct();

		int nHorzLeft = pos.horzPos.nLeft_Top; // 0
		int nHorzLeftNew = posnew.horzPos.nLeft_Top; // 300

		if(nHorzLeftNew == 0)
		{
			KillTimer(RIGHT_TIMER);
			m_bStartTimer = FALSE;
		}
		else
		{
			pos.horzPos.nLeft_Top -= 20;
			posnew.horzPos.nLeft_Top -= 20;	
		}

		pPos->SetPosStruct(pos);
		m_pLeftForm->SetPosition((IDUIPos*)pPos);
		pPosNew->SetPosStruct(posnew);
		m_pRightForm->SetPosition((IDUIPos*)pPosNew);

		m_pDirectUI->ResizeThis();
		m_pDirectUI->DirectDraw(TRUE);
	}

	else if(nIDEvent == LEFT_TIMER)
	{
		IDUIPos* pPos = NULL;
		DUIPosition pos;
		IDUIPos* pPosNew = NULL;
		DUIPosition posnew;

		pPos = (IDUIPos*)m_pLeftForm->GetPosition();
		pPosNew = (IDUIPos*)m_pRightForm->GetPosition();

		pos = pPos->GetPosStruct();
		posnew = pPosNew->GetPosStruct();

		int nHorzLeft = pos.horzPos.nLeft_Top;       //-300
		int nHorzLeftNew = posnew.horzPos.nLeft_Top; //0

		if(nHorzLeft == 0)
		{
			KillTimer(LEFT_TIMER);
			m_bStartTimer = FALSE;
		}
		else
		{
			pos.horzPos.nLeft_Top += 20;
			posnew.horzPos.nLeft_Top += 20;
		}

		pPos->SetPosStruct(pos);
		m_pLeftForm->SetPosition((IDUIPos*)pPos);
		pPosNew->SetPosStruct(posnew);
		m_pRightForm->SetPosition((IDUIPos*)pPosNew);

		m_pDirectUI->ResizeThis();
		m_pDirectUI->DirectDraw(TRUE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CMy360DesktopDlg::ChangeSkin(SKIN_TYPE eType)
{
	m_dSetMenu.ChangeSkin(eType);

	IDUIImageBase* pImage = NULL;
	if (eType == ST_BLACK_SKIN)
	{
		//优雅爵士
		pImage = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("MainBackground2"), NULL, TRUE);
	}
	else if (eType == ST_GREEN_SKIN)
	{
		//水墨风景
		pImage = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("MainBackground3"), NULL, TRUE);
	}
	else if (eType == ST_DEFAULT_SKIN)
	{
		//默认皮肤
		pImage = (IDUIImageBase*)theApp.m_pDUIRes->GetResObject(DUIOBJTYPE_IMAGEBASE, _T("MainBackground"), NULL, TRUE);
	}
	if (pImage)
	{
		m_pMainBack->SetBackImageSec(pImage);
		RECT rect;
		GetWindowRect(&rect);
		rect.bottom -= 1;
		MoveWindow(&rect, false);
		rect.bottom += 1;
		MoveWindow(&rect, false);
	}
}

void	CMy360DesktopDlg::SetSel(int nSel)
{
	m_pBtnNum1->DestroyBmpPerPixel();
	m_pBtnNum2->DestroyBmpPerPixel();
	m_pBtnNum3->DestroyBmpPerPixel();
	m_pBtnNum4->DestroyBmpPerPixel();

	m_pBtnNum1->SetStaticState(DUI_STATIC_ACTIVE);
	m_pBtnNum2->SetStaticState(DUI_STATIC_ACTIVE);
	m_pBtnNum3->SetStaticState(DUI_STATIC_ACTIVE);
	m_pBtnNum4->SetStaticState(DUI_STATIC_ACTIVE);

	switch(nSel)
	{
	case 1:
		m_pBtnNum1->SetStaticState(DUI_STATIC_INACTIVE);
		break;
	case 2:
		m_pBtnNum2->SetStaticState(DUI_STATIC_INACTIVE);
		break;
	case 3:
		m_pBtnNum3->SetStaticState(DUI_STATIC_INACTIVE);
		break;
	case 4:
		m_pBtnNum4->SetStaticState(DUI_STATIC_INACTIVE);
		break;
	}

	m_nSel = nSel;
	m_pDirectUI->DirectDraw(TRUE);
}

/* 
窗体的显示隐藏由该函数完成,参数hide决定显示还是隐藏. 
*/ 
void CMy360DesktopDlg::HideSide(BOOL hide) 
{ 
	if (!m_bHasHide && !hide)return;
	m_bHasHide = hide;

	POINT ptmove;
	GetCursorPos(&ptmove);
	CRect rc; 
	int moves = 20; //动画滚动窗体的步数,如果你觉得不够平滑,可以增大该值. 
	int xStep, yStep; 
	int xEnd, yEnd; 
	int width; 
	int height; 
	register int i; 
	GetWindowRect(&rc); 
	width = rc.right - rc.left; 
	height = rc.bottom - rc.top; 

	//下边判断窗体该如何移动,由停靠方式决定 
	switch (m_eAlignType) 
	{ 
	case ALIGN_TOP: 
		{ 
			//向上移藏 
			xStep = 0; 
			xEnd = rc.left; 
			if (hide) 
			{ 
				yStep = -rc.bottom / moves; 
				yEnd = -height + NEAR_SIDE; 
				ptmove.y += 5;
				SetCursorPos(ptmove.x, ptmove.y);
			} 
			else 
			{ 
				yStep = -rc.top / moves; 
				yEnd = 0; 
			} 
			break; 
		} 
	case ALIGN_LEFT: 
		{ 
			//向左移藏 
			yStep = 0; 
			yEnd = rc.top; 
			if (hide) 
			{ 
				xStep = -rc.right / moves; 
				xEnd = -width + NEAR_SIDE; 
				ptmove.x += 7;
				SetCursorPos(ptmove.x, ptmove.y);
			} 
			else 
			{ 
				xStep = -rc.left / moves; 
				xEnd = 0; 
			} 
			break; 
		} 
	case ALIGN_RIGHT: 
		{ 
			//向右移藏 
			yStep = 0; 
			yEnd = rc.top; 
			if (hide) 
			{ 
				xStep = (m_nScreenX - rc.left) / moves; 
				xEnd = m_nScreenX - NEAR_SIDE; 
				ptmove.x -= 5;
				SetCursorPos(ptmove.x, ptmove.y);
			} 
			else 
			{ 
				xStep = (m_nScreenX - rc.right) / moves; 
				xEnd = m_nScreenX - width; 
			} 
			break; 
		} 
	case ALIGN_BOTTOM:
		{
			//向下移藏 
			xStep = 0; 
			xEnd = rc.left; 
			xStep = 0; 
			xEnd = rc.left; 
		
			if (hide) 
			{ 
				yStep = (GetSystemMetrics(SM_CYSCREEN) - rc.top) / moves; 
				yEnd = GetSystemMetrics(SM_CYSCREEN) - NEAR_SIDE;
				ptmove.y -= 7;
				SetCursorPos(ptmove.x, ptmove.y);
			} 
			else 
			{ 
				yStep = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / moves; 
				yEnd = GetSystemMetrics(SM_CYSCREEN) - height; 
			} 
			break; 
		}
	default: 
		return; 
	} 
	//动画滚动窗体. 
	for (i = 0; i < moves; i++) 
	{ 
		rc.left += xStep; 
		rc.top += yStep; 
		SetWindowPos(NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOSENDCHANGING); 
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW); 
		Sleep(5); 
	} 
	SetWindowPos(NULL, xEnd, yEnd, 0, 0, SWP_NOSIZE); 

	if (m_dSetMenu.GetChangeWinDlg()->GetSafeHwnd() && m_dSetMenu.GetChangeWinDlg()->IsWindowVisible())
	{
		if (GetForegroundWindow()->GetSafeHwnd() != m_dSetMenu.GetChangeWinDlg()->GetSafeHwnd())
			m_dSetMenu.GetChangeWinDlg()->ShowWindow(SW_HIDE);
	}
} 

void CMy360DesktopDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT stTRACKMOUSEEVENT; 
	stTRACKMOUSEEVENT.cbSize = sizeof(stTRACKMOUSEEVENT);
	stTRACKMOUSEEVENT.hwndTrack=m_hWnd;
	stTRACKMOUSEEVENT.dwFlags=TME_LEAVE;
	_TrackMouseEvent(&stTRACKMOUSEEVENT);

	if (m_eAlignType != ALIGN_NONE)
	{
		CRect rc; 
		GetWindowRect(&rc); 
		if (rc.left < 0 || rc.top < 0 || rc.right > m_nFullScreenX || rc.bottom > m_nFullScreenY) 
			HideSide(FALSE); 
	}
	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CMy360DesktopDlg::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	if (m_eAlignType != ALIGN_NONE)
	{
		m_dLessWindow.ShowWindow(SW_HIDE);
		CRect rc; 
		GetWindowRect(&rc);
		CPoint pt;
		GetCursorPos(&pt);
		if (!rc.PtInRect(pt))
		{
			HideSide(TRUE);
			Sleep(600);
		}
	}

	return 0;
}
void CMy360DesktopDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetWindowRect(&rect);

	if (GetWindowLong(m_hWnd,GWL_STYLE)  & WS_MAXIMIZE)
		return; 
	if (rect.left != x || rect.top != y)return;

	if (m_dSetMenu.GetChangeWinDlg()->IsWindowVisible())
	{
		if (x < 390)
		{
			CRect rc;
			m_dSetMenu.GetChangeWinDlg()->GetWindowRect(&rc);
			rc.MoveToXY((x+318), (y+71));
			m_dSetMenu.GetChangeWinDlg()->m_pSknCloseBtn->SetVisible(FALSE, TRUE,FALSE);
			m_dSetMenu.GetChangeWinDlg()->m_pSknCloseBtnRight->SetVisible(TRUE, TRUE,FALSE);
			m_dSetMenu.GetChangeWinDlg()->MoveWindow(&rc, TRUE);
		}
		else
		{
			CRect rc;
			m_dSetMenu.GetChangeWinDlg()->GetWindowRect(&rc);

			m_dSetMenu.GetChangeWinDlg()->m_pSknCloseBtnRight->SetVisible(FALSE, TRUE,FALSE);
			m_dSetMenu.GetChangeWinDlg()->m_pSknCloseBtn->SetVisible(TRUE, TRUE,FALSE);

			rc.MoveToXY((x-389), (y+71));
			m_dSetMenu.GetChangeWinDlg()->MoveWindow(&rc, TRUE);
		}
	}

	if(!m_bPress)return;

	if (m_eAlignType == ALIGN_NONE) 
	{ 
		POINT ptMove;
		GetCursorPos(&ptMove);

		//在上方有效地距离内停靠
		if (ptMove.y < 4) 
		{
			::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			::SetWindowPos(m_dLessWindow, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			m_eAlignType = ALIGN_TOP;
			m_dLessWindow.ShowAlphaWindow(ALIGN_TOP);
		}
		//在下方有效地距离内停靠
		else if (ptMove.y > m_nFullScreenY - 40)
		{
			::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			::SetWindowPos(m_dLessWindow, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			m_eAlignType = ALIGN_BOTTOM;	
			m_dLessWindow.ShowAlphaWindow(ALIGN_BOTTOM);

		}

		////在左边有效地距离内停靠
		if (ptMove.x < 40)
		{
			::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			::SetWindowPos(m_dLessWindow, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			m_eAlignType = ALIGN_LEFT; 
			m_dLessWindow.ShowAlphaWindow(ALIGN_LEFT);
		}
		//在右边有效地距离内停靠
		else if (ptMove.x > m_nFullScreenX - 40)
		{
			::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			::SetWindowPos(m_dLessWindow, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
			m_eAlignType = ALIGN_RIGHT;
			m_dLessWindow.ShowAlphaWindow(ALIGN_RIGHT);
		}
	} 
	else
	{ 
		POINT ptMove;
		GetCursorPos(&ptMove);
		if (m_eAlignType == ALIGN_TOP) 
		{ 
			if (ptMove.y > 4)
			{
				::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
				::SetWindowPos(m_dLessWindow, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
				m_eAlignType = ALIGN_NONE; 
				m_dLessWindow.ShowWindow(SW_HIDE);
			}
		} 
		if (m_eAlignType == ALIGN_LEFT) 
		{ 
			if (ptMove.x > 40) 
			{
				::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
				::SetWindowPos(m_dLessWindow, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
				m_eAlignType = ALIGN_NONE; 
				m_dLessWindow.ShowWindow(SW_HIDE);
			}
		} 
		else if (m_eAlignType == ALIGN_RIGHT) 
		{ 
			if ( ptMove.x < m_nFullScreenX - 40)
			{
				::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
				::SetWindowPos(m_dLessWindow, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
				m_eAlignType = ALIGN_NONE; 
				m_dLessWindow.ShowWindow(SW_HIDE);
			}
		} 
		else if (m_eAlignType == ALIGN_BOTTOM)
		{
			if (ptMove.y < m_nFullScreenY - 40)
			{
				::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
				::SetWindowPos(m_dLessWindow, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
				m_eAlignType = ALIGN_NONE;
				m_dLessWindow.ShowWindow(SW_HIDE);
			}
		}
	} 
}

void CMy360DesktopDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	if (m_dSetMenu.GetChangeWinDlg()->GetSafeHwnd() && m_dSetMenu.GetChangeWinDlg()->IsWindowVisible())
	{
		if (GetForegroundWindow()->GetSafeHwnd() != m_dSetMenu.GetChangeWinDlg()->GetSafeHwnd())
		m_dSetMenu.GetChangeWinDlg()->ShowWindow(SW_HIDE);
	}
}

void CMy360DesktopDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	IDUIRect* pRect = m_pHotAppListView->GetRect();
	SkinRect skinRc = pRect->GetRectStruct();
	CRect rect(skinRc.left, skinRc.top, skinRc.right, skinRc.bottom);
	if(rect.PtInRect(point))return;

	SetCapture();
	GetCursorPos(&m_ptOld);
	m_bPress = TRUE;

	CDialog::OnLButtonDown(nFlags, point);
}

void CMy360DesktopDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReleaseCapture();
	m_bPress = FALSE;

	if (m_eAlignType != ALIGN_NONE)
	{
		m_dLessWindow.ShowWindow(SW_HIDE);
		HideSide(TRUE);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

LRESULT CMy360DesktopDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(message)
	{
	case WM_MOUSEMOVE:
		if (MK_LBUTTON  == wParam)
		{
			if (m_bPress == FALSE)
			{
				return 0;
			}
				
			CPoint ptNew;
			GetCursorPos(&ptNew);
			if (ptNew != m_ptOld)
			{
				CPoint offset;
				offset = ptNew - m_ptOld;
				m_ptOld = ptNew;
				CRect rc;
				GetWindowRect(&rc);
				rc.OffsetRect(offset);
				SetWindowPos(NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE);
			}
		}
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
