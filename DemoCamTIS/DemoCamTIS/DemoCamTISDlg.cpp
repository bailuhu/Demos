// Open DeviceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DemoCamTIS.h"
#include "DemoCamTISDlg.h"

#include <windef.h>
#include <atlimage.h>
#include <iostream>

using namespace DShowLib;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// COpenDeviceDlg dialog



COpenDeviceDlg::COpenDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenDeviceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICVIDEO, m_cStaticVideoWindow);
}

BEGIN_MESSAGE_MAP(COpenDeviceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// COpenDeviceDlg message handlers

BOOL COpenDeviceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	// Assign this window to the Grabber object for live video display.
//	grabber.setHWND(m_cStaticVideoWindow.m_hWnd);
// 	grabber.openDev( "DMK 38UX304" );
// 
// 	if( grabber.isDevValid())
// 			grabber.startLive();

	
	DShowLib::Grabber::tVidCapDevListPtr pVidCapDevList = grabber.getAvailableVideoCaptureDevices();
	if (pVidCapDevList == 0 || pVidCapDevList->empty())
	{
		
		return -1; // No device available.
	}
	DShowLib::VideoCaptureDeviceItem videoCapDevItem = pVidCapDevList->at(0);
	CString szInfo;
	szInfo.Format(L"cam number: %d\ngetName: %s\n getBaseName: %s\n getUniqueName: %s\n getDisplayName: %s\n ", 
		pVidCapDevList->size(), 
		CString(videoCapDevItem.getName().c_str()),
		CString(videoCapDevItem.getBaseName().c_str()),
		CString(videoCapDevItem.getUniqueName().c_str()),
		CString(videoCapDevItem.getDisplayName().c_str()),
		CString(videoCapDevItem.getDisplayName().c_str()),
		CString(videoCapDevItem.getDisplayName().c_str())
		);
	//AfxMessageBox(szInfo);
	szInfo = L"";


	if (grabber.openDev(pVidCapDevList->at(0)))
	{
		// open success
	}
	else
	{
		// open fail
	}
	

	// Check whether video norms are available with the current video capture device.
	if (grabber.isVideoNormAvailableWithCurDev())
	{
		// Query for all available video norms.
		Grabber::tVidNrmListPtr pVidNrmList = grabber.getAvailableVideoNorms();

		if (pVidNrmList == 0)
		{
			std::cerr << "Error: " << grabber.getLastError().toString() << std::endl;
			return -1;
		}
		
		grabber.setVideoNorm(pVidNrmList->at(0));
	}


	Grabber::tVidFmtListPtr pVidFmtList = grabber.getAvailableVideoFormats();
	if (pVidFmtList == 0) // No video formats available?
	{
		std::cerr << "Error : " << grabber.getLastError().toString() << std::endl;
	}
	else
	{
		unsigned int counter = 0;
		// List the available video formats.
		for (Grabber::tVidFmtList::iterator it = pVidFmtList->begin();
			it != pVidFmtList->end();
			++it)
		{
			CString temp;
			temp.Format(L"\t[%d] %s\n", counter++, CString(it->toString().c_str()));
			szInfo += temp;
		//	std::cout << "\t[" << counter++ << "] " << it->toString() << std::endl;
		}
	//	AfxMessageBox(szInfo);
	}


// 	Grabber grabber;
// 	if (!setupDeviceFromFile(grabber))
// 	{
// 		return -1;
// 	}
	// Set the image buffer format to eY800. eY800 means monochrome, 8 bits (1 byte) per pixel.
	// Let the sink create a matching MemBufferCollection with 1 buffer.
	tFrameHandlerSinkPtr pSink = FrameHandlerSink::create(eY800, 1);

	// We use snap mode.
	pSink->setSnapMode(true);

	// Set the sink.
	grabber.setSinkType(pSink);

	// Prepare the live mode, to get the output size if the sink.
	if (!grabber.prepareLive(false))
	{
		std::cerr << "Could not render the VideoFormat into a eY800 sink.";
		return -1;
	}

	// Retrieve the output type and dimension of the handler sink.
	// The dimension of the sink could be different from the VideoFormat, when
	// you use filters.
	FrameTypeInfo info;
	pSink->getOutputFrameType(info);
// 	BYTE* pBuf[5];
// 	// Allocate 5 image buffers of the above calculate buffer size.
// 	for (int i = 0; i < 5; ++i)
// 	{
// 		pBuf[i] = new BYTE[info.buffersize];
// 	}
// 
// 	// Create a new MemBuffer collection that uses our own image buffers.
// 	tMemBufferCollectionPtr pCollection = MemBufferCollection::create(info, 5, pBuf);
// 	if (pCollection == 0 || !pSink->setMemBufferCollection(pCollection))
// 	{
// 		std::cerr << "Could not set the new MemBufferCollection, because types do not match.";
// 		return -1;
// 	}

	DShowLib::VideoFormatItem videoFormatItem = grabber.getVideoFormat();
	videoFormatItem.getSize().cx;
	int w = 4096;
	int h = 3000;
	grabber.startLive(false);

	CImage img;
	img.Load(L"C:\\Users\\simpl\\Desktop\\Temp\\SaveImg.bmp");
	BYTE *destPtr = (BYTE*)img.GetBits();
	int destPitch = img.GetPitch();
	BYTE* pBuffer= nullptr;
	
	int cnt = 0;
	do 
	{
		pSink->snapImages(1);
		
		

		pBuffer = pSink->getLastAcqMemBuffer()->getPtr();

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				destPtr[i*destPitch + j + 0] = pBuffer[i*w + j];
// 				destPtr[i*destPitch + j + 1] = pBuffer[i*w + j];
// 				destPtr[i*destPitch + j + 2] = pBuffer[i*w + j];
			}

		}
		//memcpy(destPtr, pBuffer, w*h*sizeof(BYTE));
		CString szTemp;
		szTemp.Format(L"C:\\Users\\simpl\\Desktop\\Temp\\%d.bmp", cnt);
		img.Save(szTemp);
		szTemp.Format(L"C:\\Users\\simpl\\Desktop\\Temp\\%ds.bmp", cnt);
		pSink->getLastAcqMemBuffer()->save(szTemp);

		
		AfxMessageBox(szTemp);
	} while (cnt++ < 10);

	grabber.stopLive();
	grabber.closeDev();
// 
// 	// Save the five captured images in the MemBuffer collection to separate files.
// 	pCollection->save("file*.bmp");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COpenDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COpenDeviceDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COpenDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
