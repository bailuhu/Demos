
// OpenGLDlgDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
// ʵ��
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


// COpenGLDlgDlg �Ի���




COpenGLDlgDlg::COpenGLDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenGLDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nGLPixelIndex = 0;
	m_hGLContext = NULL;
	//�����Ƿ�ɹ���ʼ��OpenGL
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


// COpenGLDlgDlg ��Ϣ�������

BOOL COpenGLDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	HDC hdc = GetDC()->GetSafeHdc();
	if (SetWindowPixelFormat(hdc))
	{
		m_bGLInitialized = CreateGLContext(hdc);
	}

	//����������䣨OnSize����Ҳ����Щ�������Ǳ�Ҫ��
	//�����ڳ���һ��ʼ�����е�ʱ��OnSize������Ӧ�ĺ�������Ч��ʹ�ó�����Ĭ������ϵΪƽ��ֱ�����������Ϊ�Ի���ͻ��������ģ�����ϵ�ĳ���Ϊ[-1,1])
	if (m_bGLInitialized)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COpenGLDlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
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
		//�����ӿ�
		glViewport(0,0,cx, cy);

		//����
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0,500.0,0.0,500.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//ѡ����GL_BACK�������ͼ�Σ���Ϊ��SetWindowPixelFormat�������趨��ʹ��˫���棨PFD_DOUBLEBUFFER��
		glDrawBuffer(GL_BACK);
		//����WM_PAINT��Ϣ���������»��ƣ�����FALSE��ʾ����ҪWindowsˢ�±�������Ϊ������OpenGL��glClear������ˢ�±���
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COpenGLDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	{//����ʧ��
		return FALSE;
	}
	if(wglMakeCurrent(hdc,this->m_hGLContext)==FALSE)
	{//ѡΪ��ǰRCʧ��
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
