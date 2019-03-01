
// DemoCVDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DemoCV.h"
#include "DemoCVDlg.h"
#include "afxdialogex.h"

#include <highgui.hpp>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDemoCVDlg 对话框




CDemoCVDlg::CDemoCVDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemoCVDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDemoCVDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDemoCVDlg::OnBnClickedReadimg)
	ON_BN_CLICKED(IDC_BUTTON2, &CDemoCVDlg::OnBnClickedEdgedetect)
END_MESSAGE_MAP()


// CDemoCVDlg 消息处理程序

BOOL CDemoCVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_PIC) ->GetClientRect( &m_rect );
	m_image = cvCreateImage( CvSize(m_rect.Width(), m_rect.Height()), IPL_DEPTH_8U, IMAGE_CHANNELS );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDemoCVDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDemoCVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDemoCVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemoCVDlg::OnBnClickedReadimg()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, _T("*.jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, NULL, NULL);										// 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("Open Image");	// 打开文件对话框的标题名
	dlg.m_ofn.lpstrFilter = _T("All Files (*.*)\0*.*\0\0");
	if( dlg.DoModal() != IDOK )					// 判断是否获得图片
		return;

	CString path = dlg.GetPathName();			// 获取图片路径
	USES_CONVERSION;
	IplImage* ipl = cvLoadImage(T2A(path), 1);	// 读取图片、缓存到一个局部变量 ipl 中
	if( !ipl )	return;
	if( m_image )								// 对上一幅显示的图片数据清零
		cvZero( m_image );

	ResizeImage( ipl );	// 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 m_image 中
	ShowImage(m_image, IDC_PIC );				// 调用显示图片函数	
	cvReleaseImage( &ipl );						// 释放 ipl 占用的内存
}

void CDemoCVDlg::ShowImage( IplImage* img, UINT ID )	// ID 是Picture Control控件的ID号
{
	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC ->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作

	SetRect( m_rect, 0, 0, m_rect.Width(), m_rect.Height() );
	CvvImage imgCpy;
	imgCpy.CopyOf(img);							// 复制图片
	imgCpy.DrawToHDC(hDC, &m_rect);				// 将图片绘制到显示控件的指定区域内

	ReleaseDC(pDC);
}

void CDemoCVDlg::OnBnClickedEdgedetect()
{
	// TODO: Add your control notification handler code here
	IplImage *gray = 0, *edge = 0;
	gray = cvCreateImage( cvSize(m_rect.Width(), m_rect.Height()), IPL_DEPTH_8U, 1 );
	edge = cvCreateImage( cvSize(m_rect.Width(), m_rect.Height()), IPL_DEPTH_8U, 1 );
	cvCvtColor( m_image, gray, CV_BGR2GRAY );
	cvCanny( gray, edge, 30, 100, 3 );
	cvCvtColor( edge, m_image, CV_GRAY2BGR );	
	ShowImage( m_image, IDC_PIC );			// 调用显示图片函数

	cvReleaseImage( &gray );
	cvReleaseImage( &edge );
}

void CDemoCVDlg::ResizeImage(IplImage* img)
{
	// 设置 m_image 的 ROI 区域，用来存入图片 img
	cvSetImageROI( m_image, cvRect(0, 0, m_rect.Width(), m_rect.Height()));

	// 对图片 img 进行缩放，并存入到 m_image 中
	cvResize( img, m_image );

	// 重置 m_image 的 ROI 准备读入下一幅图片
	cvResetImageROI( m_image );
}
