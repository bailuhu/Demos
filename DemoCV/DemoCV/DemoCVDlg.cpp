
// DemoCVDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DemoCV.h"
#include "DemoCVDlg.h"
#include "afxdialogex.h"

#include <highgui.hpp>
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


// CDemoCVDlg �Ի���




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


// CDemoCVDlg ��Ϣ�������

BOOL CDemoCVDlg::OnInitDialog()
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
	GetDlgItem(IDC_PIC) ->GetClientRect( &m_rect );
	m_image = cvCreateImage( CvSize(m_rect.Width(), m_rect.Height()), IPL_DEPTH_8U, IMAGE_CHANNELS );

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDemoCVDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDemoCVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemoCVDlg::OnBnClickedReadimg()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, _T("*.jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, NULL, NULL);										// ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("Open Image");	// ���ļ��Ի���ı�����
	dlg.m_ofn.lpstrFilter = _T("All Files (*.*)\0*.*\0\0");
	if( dlg.DoModal() != IDOK )					// �ж��Ƿ���ͼƬ
		return;

	CString path = dlg.GetPathName();			// ��ȡͼƬ·��
	USES_CONVERSION;
	IplImage* ipl = cvLoadImage(T2A(path), 1);	// ��ȡͼƬ�����浽һ���ֲ����� ipl ��
	if( !ipl )	return;
	if( m_image )								// ����һ����ʾ��ͼƬ��������
		cvZero( m_image );

	ResizeImage( ipl );	// �Զ����ͼƬ�������ţ�ʹ��������ֵ�߸պõ��� 256���ٸ��Ƶ� m_image ��
	ShowImage(m_image, IDC_PIC );				// ������ʾͼƬ����	
	cvReleaseImage( &ipl );						// �ͷ� ipl ռ�õ��ڴ�
}

void CDemoCVDlg::ShowImage( IplImage* img, UINT ID )	// ID ��Picture Control�ؼ���ID��
{
	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// �����ʾ�ؼ��� DC
	HDC hDC = pDC ->GetSafeHdc();				// ��ȡ HDC(�豸���) �����л�ͼ����

	SetRect( m_rect, 0, 0, m_rect.Width(), m_rect.Height() );
	CvvImage imgCpy;
	imgCpy.CopyOf(img);							// ����ͼƬ
	imgCpy.DrawToHDC(hDC, &m_rect);				// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������

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
	ShowImage( m_image, IDC_PIC );			// ������ʾͼƬ����

	cvReleaseImage( &gray );
	cvReleaseImage( &edge );
}

void CDemoCVDlg::ResizeImage(IplImage* img)
{
	// ���� m_image �� ROI ������������ͼƬ img
	cvSetImageROI( m_image, cvRect(0, 0, m_rect.Width(), m_rect.Height()));

	// ��ͼƬ img �������ţ������뵽 m_image ��
	cvResize( img, m_image );

	// ���� m_image �� ROI ׼��������һ��ͼƬ
	cvResetImageROI( m_image );
}
