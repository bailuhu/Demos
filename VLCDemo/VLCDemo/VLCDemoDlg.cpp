
// VCLDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VLCDemo.h"
#include "VLCDemoDlg.h"
#include "afxdialogex.h"

#include "Player.h"
#include "DlgURL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVCLDemoDlg dialog



CVCLDemoDlg::CVCLDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVCLDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCLDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_listFile);
	DDX_Control(pDX, IDC_STATIC_VEDIO, m_vedio);
	DDX_Control(pDX, IDC_BTN_PLAY, m_btnPlayPause);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_SLIDER_TIME, m_slider);
	DDX_Control(pDX, IDC_STATIC_TIME, m_time);
	DDX_Control(pDX, IDC_BTN_FILE, m_btnFile);
	DDX_Control(pDX, IDC_BTN_URL, m_btnURL);
}

BEGIN_MESSAGE_MAP(CVCLDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_PLAY, &CVCLDemoDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CVCLDemoDlg::OnBnClickedBtnStop)
	ON_LBN_DBLCLK(IDC_LIST_FILE, &CVCLDemoDlg::OnLbnDblclkListFile)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_FILE, &CVCLDemoDlg::OnBnClickedBtnFile)
	ON_BN_CLICKED(IDC_BTN_URL, &CVCLDemoDlg::OnBnClickedBtnUrl)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CVCLDemoDlg message handlers

BOOL CVCLDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	if (!m_pPlayer)
		m_pPlayer = new Player;
	m_pPlayer->init();
	
	SendMessage(WM_SIZE, 0, 0);
	//初始化slider
	m_slider.SetRange(0, 100);
	SetTimer(1, 1000, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCLDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVCLDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVCLDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVCLDemoDlg::OnDropFiles(HDROP hDropInfo)
{
	wchar_t szFileName[MAX_PATH] = { 0 };
	UINT nFileCount = DragQueryFile(hDropInfo, -1, nullptr, 0);
	for (UINT i = 0; i < nFileCount; i++)
	{
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
	//	PWSTR szTemp = StrChrW(szFileName, '\\')+1;//只显示文件名称
		m_listFile.AddString(szFileName);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


void CVCLDemoDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	

}

/*
*	@这个函数会在窗口大小改变结束后被调用，通常会在这个函数里重新摆放各个控件的位置及大小。
*	@用这个函数暂时没有找到什么办法可以控制窗口的最大最小大小。  
*/
void CVCLDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (!m_listFile) return;

	CRect listRect, clientRect, vedioRect, btnRect;
	GetClientRect(&clientRect);
	m_listFile.GetWindowRect(&listRect);
	m_vedio.GetWindowRect(&vedioRect);
	m_btnPlayPause.GetWindowRect(btnRect);
	int cw = clientRect.Width();
	int ch = clientRect.Height();

	int listW = cw / 10 < 100 ? 100 : cw / 10;//最小200
	int btnH = ch / 10 > 50 ? 50 : ch / 10;//最大50
	int sliderH = 20;
	int bottomH = btnH + sliderH;

	m_listFile.MoveWindow(cw - listW, 0, listW, ch - bottomH);

	m_slider.MoveWindow(0, ch - bottomH, cw - listW, sliderH);
	m_time.MoveWindow(cw - listW, ch - bottomH, listW, sliderH);

	int btnNum = 4;
	m_btnPlayPause.MoveWindow(0, ch - btnH, cw / btnNum, btnH);
	m_btnStop.MoveWindow(cw * 1 / btnNum, ch - btnH, cw / btnNum, btnH);
	m_btnFile.MoveWindow(cw / 2, ch - btnH, cw / btnNum, btnH);
	m_btnURL.MoveWindow(cw * 3 / btnNum, ch - btnH, cw / btnNum, btnH);

	m_vedio.MoveWindow(0, 0, cw - listW, ch - bottomH);

	InvalidateRect(clientRect);
}


void CVCLDemoDlg::OnBnClickedBtnPlay()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_listFile.GetCurSel();
	if (-1 == nIndex) return;

	wchar_t szPath[MAX_PATH] = { 0 };
	m_listFile.GetText(nIndex, szPath);

	CString szText;
	m_btnPlayPause.GetWindowTextW(szText);
	if (szText == "Play")
	{
		m_pPlayer->set_play_wnd(m_vedio.GetSafeHwnd());
		m_pPlayer->play(CW2A(szPath));
		m_btnPlayPause.SetWindowTextW(L"Pause");
	}
	else
	{
		m_pPlayer->pause();
		m_btnPlayPause.SetWindowTextW(L"Play");
	}

}


void CVCLDemoDlg::OnBnClickedBtnStop()
{
	m_pPlayer->stop();
	m_btnPlayPause.SetWindowTextW(L"Play");
}


void CVCLDemoDlg::OnLbnDblclkListFile()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_listFile.GetCurSel();
	wchar_t szPath[MAX_PATH] = { 0 };
	m_listFile.GetText(nIndex, szPath);
	m_pPlayer->set_play_wnd(m_vedio.GetSafeHwnd());
	m_pPlayer->play(CW2A(szPath));
	m_btnPlayPause.SetWindowTextW(L"Pause");
}	


void CVCLDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//设置slider 进度百分比
	m_slider.SetPos((int)(m_pPlayer->get_position() * 100));

	//设置进度 当前时间/总时间
	int curPos = (int)(m_pPlayer->get_position() * m_pPlayer->get_length());

	int64_t len = m_pPlayer->get_length();
	USHORT hour = (USHORT)(len / 3600);
	USHORT minute = (USHORT)(len - hour * 3600) / 60;
	USHORT second = (USHORT)(len - hour * 3600 - minute * 60);

	USHORT curH = (USHORT)(curPos / 3600);
	USHORT curM = (USHORT)(curPos - 3600 * curH) / 60;
	USHORT curS = (USHORT)(curPos - 3600 * curH - 60 * curM);

	CString szTemp;
	szTemp.Format(L"%02d:%02d:%02d/%02d:%02d:%02d", curH,curM,curS, hour, minute, second);
	m_time.SetWindowTextW(szTemp);

	CDialogEx::OnTimer(nIDEvent);
}


void CVCLDemoDlg::OnBnClickedBtnFile()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_HIDEREADONLY, L"All File(*.*)|*.*||");
	dlg.m_ofn.nMaxFile = 500 * MAX_PATH;
	wchar_t* pathNames = new wchar_t[dlg.m_ofn.nMaxFile];
	dlg.m_ofn.lpstrFile = pathNames;
	ZeroMemory(dlg.m_ofn.lpstrFile, sizeof(wchar_t) * dlg.m_ofn.nMaxFile);	//对内存块清零  

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos_file;
		pos_file = dlg.GetStartPosition();
		while (pos_file != 0)
		{
			CString szPath = dlg.GetNextPathName(pos_file);
			m_listFile.AddString(szPath);
		}
	}
}


void CVCLDemoDlg::OnBnClickedBtnUrl()
{
	// TODO: Add your control notification handler code here
//	char* urlPath = "rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp";
	char* urlPath = "rtsp://218.204.223.237:554/live/1/67A7572844E51A64/f68g2mj7wjua3la7.sdp";

	DlgURL dlg;
	if (dlg.DoModal() == IDOK)
	{
		USES_CONVERSION;
		urlPath = T2A(dlg.m_szUrl);
	}
	
	m_pPlayer->set_play_wnd(m_vedio.GetSafeHwnd());
	m_pPlayer->play_url(urlPath);
}


HBRUSH CVCLDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(COLOR_BLACK);	//设置字体背景
		pDC->SetTextColor(COLOR_BLACK);		//设置字体前景
		hbr = CreateSolidBrush(COLOR_GREY);	//设置背景
		break;
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(COLOR_LIGHT_BLUE);	//设置字体背景
		pDC->SetTextColor(COLOR_BLACK);		//设置字体前景
		hbr = CreateSolidBrush(COLOR_LIGHT_BLUE);	//设置背景
		break;
	case CTLCOLOR_DLG:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(COLOR_GREY21);	//设置字体背景
		pDC->SetTextColor(COLOR_LIGHT_GREEN);		//设置字体前景
		hbr = CreateSolidBrush(COLOR_GREY21);	//设置背景
		break;
	default:
		break;
	}
	return hbr;
}
