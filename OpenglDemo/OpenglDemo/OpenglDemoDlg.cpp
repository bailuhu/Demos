
// OpenGLDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenglDemo.h"
#include "OpenglDemoDlg.h"
#include "afxdialogex.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


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


// COpenGLDlgDlg 对话框




COpenGLDlgDlg::COpenGLDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenGLDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nGLPixelIndex = 0;
	m_hGLContext = NULL;
	//表明是否成功初始化OpenGL
	m_bGLInitialized = FALSE;
}

void COpenGLDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenGLDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// COpenGLDlgDlg 消息处理程序

BOOL COpenGLDlgDlg::OnInitDialog()
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

	HDC hdc = GetDC()->GetSafeHdc();
	if (SetWindowPixelFormat(hdc))
	{
		m_bGLInitialized = CreateGLContext(hdc);
	}

	//下面的五个语句（OnSize函数也有这些函数）是必要的
	//否则在程序一开始的运行的时候，OnSize函数相应的函数会无效（使得场景的默认坐标系为平面直角坐标的中心为对话框客户区的中心，坐标系的长度为[-1,1])
	if (m_bGLInitialized)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COpenGLDlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (wglGetCurrentContext() != NULL)
	{
		wglMakeCurrent(NULL, NULL);
	}
	if (NULL != m_hGLContext)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
}

void COpenGLDlgDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (SIZE_MINIMIZED != nType && m_bGLInitialized)
	{
		//设置视口
		glViewport(0,0,cx, cy);

		//设置
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0,500.0,0.0,500.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//选择在GL_BACK缓存绘制图形，因为在SetWindowPixelFormat函数中设定了使用双缓存（PFD_DOUBLEBUFFER）
		glDrawBuffer(GL_BACK);
		//发出WM_PAINT消息，引起重新绘制，参数FALSE表示不需要Windows刷新背景，因为利用了OpenGL的glClear函数来刷新背景
		Invalidate(FALSE);
	}
}

void COpenGLDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COpenGLDlgDlg::OnPaint()
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
		CPaintDC dc(this);
		if (m_bGLInitialized)
		{
			glClearColor(0.0f,0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//glBegin(GL_POLYGON);
			//glBegin(GL_TRIANGLES);
			glBegin(GL_LINE_STRIP);
			glColor4f(1.0f,0.0f,0.0f,1.0f);
			glVertex2f(100.0f,50.0f);
			//glVertex2f(0.0f,0.0f);
			glColor4f(0.0f,1.0f,0.0f,1.0f);
			glVertex2f(450.0f,400.0f);
			glColor4f(0.0f,0.0f,1.0f,1.0f);
			glVertex2f(450.0f,50.0f);
			glEnd();
			glFlush();

			SwapBuffers(dc.GetSafeHdc());
		}
		CDialogEx::OnPaint();
	}	
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenGLDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL COpenGLDlgDlg::CreateGLContext(HDC hdc)
{
	/*if ((m_hGLContext = wglCreateContext(hdc)) != NULL)
	{
		return wglMakeCurrent(hdc, m_hGLContext);
	}
	return FALSE;*/

	this->m_hGLContext = wglCreateContext(hdc);
	if(this->m_hGLContext==NULL)
	{//创建失败
		return FALSE;
	}
	if(wglMakeCurrent(hdc,this->m_hGLContext)==FALSE)
	{//选为当前RC失败
		return FALSE;
	}
	return TRUE;
}

BOOL COpenGLDlgDlg::SetWindowPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,
		32,
		0,0,
		PFD_MAIN_PLANE,
		0,0,0,0
	};
	if ((m_nGLPixelIndex = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		m_nGLPixelIndex = 1;
		if (DescribePixelFormat(hdc, m_nGLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
		{
			return FALSE;
		}
	}

	return SetPixelFormat(hdc, m_nGLPixelIndex, &pfd);
}
