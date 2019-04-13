
// TcpDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpDemo.h"
#include "TcpDemoDlg.h"
#include "afxdialogex.h"

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


// CTcpDemoDlg �Ի���




CTcpDemoDlg::CTcpDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTcpDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CmdID = 101;
	Status = -1;
	Result = -1;
	ErrInfo = L"ErrInfo";
	TestData = L"TestData";
	m_bAcceptConnectLoop = FALSE;
	m_tcpMode = TM_Server_CameraTest;
	m_jogVel = 30.0;
	// this is the test branch
}

void CTcpDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, CmdID);
	DDX_Text(pDX, IDC_EDIT2, Status);
	DDX_Text(pDX, IDC_EDIT3, Result);
	DDX_Text(pDX, IDC_EDIT4, ErrInfo);
	DDX_Text(pDX, IDC_EDIT5, TestData);
	DDX_Text(pDX, IDC_EDIT_JOG_VEL, m_jogVel);
	DDX_Control(pDX, IDC_TXT_SHOW_SEND_MSG, ShowSendMsg);
	DDX_Control(pDX, IDC_TXT_SHOW_RECV_MSG, ShowRecvMsg);
	DDX_Radio(pDX, IDC_RADIO_SERVER, m_tcpMode);
	DDX_Control(pDX, IDC_COMBO_AXIS, m_combAxis);
}

BEGIN_MESSAGE_MAP(CTcpDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTcpDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_SERVER, &CTcpDemoDlg::OnBnClickedRadioServer)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, &CTcpDemoDlg::OnBnClickedRadioClient)
	ON_BN_CLICKED(IDC_RADIO_CLIENT_LENS_AA, &CTcpDemoDlg::OnBnClickedRadioClientLensAa)
	ON_CBN_SELCHANGE(IDC_COMBO_AXIS, &CTcpDemoDlg::OnCbnSelchangeComboAxis)
	ON_BN_CLICKED(IDC_BTN_HOME, &CTcpDemoDlg::OnBnClickedBtnHome)
	ON_BN_CLICKED(IDC_BTN_JOG_P, &CTcpDemoDlg::OnBnClickedBtnJogP)
	ON_BN_CLICKED(IDC_BTN_JOG_N, &CTcpDemoDlg::OnBnClickedBtnJogN)
	ON_BN_CLICKED(IDC_BTN_ENABLE, &CTcpDemoDlg::OnBnClickedBtnEnable)
	ON_BN_CLICKED(IDC_BTN_ABS_MOVE, &CTcpDemoDlg::OnBnClickedBtnAbsMove)
	ON_BN_CLICKED(IDC_BTN_REL_MOVE, &CTcpDemoDlg::OnBnClickedBtnRelMove)
	ON_BN_CLICKED(IDC_BTN_REL_MOVE2, &CTcpDemoDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_STOP, &CTcpDemoDlg::OnBnClickedBtnStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_AUTO_START, &CTcpDemoDlg::OnBnClickedBtnAutoStart)
END_MESSAGE_MAP()


// CTcpDemoDlg ��Ϣ�������

BOOL CTcpDemoDlg::OnInitDialog()
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
	m_combAxis.InsertString(0, L"X��");
	m_combAxis.InsertString(1, L"Y��");
	m_combAxis.InsertString(2, L"Z1��");
	m_combAxis.InsertString(3, L"Z2��");
	m_combAxis.InsertString(4, L"Z3��");
	m_combAxis.InsertString(5, L"W��");
	m_combAxis.SetCurSel(0);


	GetDlgItem(IDC_EDIT_ACC)->SetWindowText(L"10.0");
	GetDlgItem(IDC_EDIT_DCC)->SetWindowText(L"10.0");
	GetDlgItem(IDC_EDIT_SPEED)->SetWindowText(L"30.0");
	GetDlgItem(IDC_EDIT_TARGET_POS)->SetWindowText(L"1.0");

	switch (m_tcpMode)
	{
		case TM_Client_CameraTest:
		{
			if (m_tcpClient.Init() == SE_OK)
			{
				int nRet = m_tcpClient.Connect(L"127.0.0.1", L"60000");
				if (nRet == SE_OK)
				{
					m_tcpClient.m_bConnected = TRUE;
					m_tcpClient.SetSocketOption(1024 * 8, 500, 100);
					m_tcpClient.SetNonBlockMode(TRUE);  //������ģʽ

					AfxBeginThread(Thread_ListenClient, this);
					m_bAcceptConnectLoop = TRUE;
				}
				else
				{
					m_tcpClient.m_bConnected = FALSE;
				}

			}
			break;
		}
		case TM_Server_CameraTest:
		{
			if (m_tcpServer.Init(L"60000", L"127.0.0.1") == SE_OK)
			{
				m_tcpServer.SetSocketOption(1024 * 8, 1000, 10000);
				m_tcpServer.SetNonBlockMode(TRUE);  //������ģʽ
				m_tcpServer.m_bConnected = FALSE;
				if (m_bAcceptConnectLoop == FALSE)
				{
					m_bAcceptConnectLoop = TRUE;
					AfxBeginThread(Thread_AcceptConnect, this);
					AfxBeginThread(Thread_Listen, this);
				}
			}

			break;
		}
		case TM_Client_Lens:
		{
			if (m_lensAA.OpenFins(L"192.168.250.1", 9600) == 0)
			{
				// init client ok
				SetTimer(2, 200, nullptr);
				SetTimer(3, 200, nullptr);
				SetTimer(4, 200, nullptr);
			}
			break;
		}
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTcpDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTcpDemoDlg::OnPaint()
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
HCURSOR CTcpDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTcpDemoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	switch (m_tcpMode)
	{
	case TM_Client_CameraTest:
		SendMsgClient(CmdID, Status, Result, ErrInfo, TestData);
		break;
	case TM_Client_Lens:
		m_lensAA.SendDword(4, 256);
		break;
	case TM_Server_CameraTest:
		SendMsg(CmdID, Status, Result, ErrInfo, TestData);
		break;
	}
}



UINT CTcpDemoDlg::Thread_AcceptConnect(LPVOID pParam)
{
	CTcpDemoDlg* pThis = (CTcpDemoDlg*)pParam;
	return pThis->Thread_AcceptConnect();
}

int CTcpDemoDlg::Thread_AcceptConnect(void)
{
	int i = 0;
	while(1)
	{	
		wstring IP;
		USHORT port;
		SOCKET sockConn;

		if (m_tcpServer.Accept(IP, port, sockConn) == SE_OK)
		{
			m_sock = sockConn;
			m_tcpServer.m_bConnected = TRUE;
 			ShowSendMsg.SetWindowText(L"���ӳɹ�");
			i++;
		}

		Sleep(100);
	}

	return 0;
}


UINT CTcpDemoDlg::Thread_Listen(LPVOID pParam)
{
	CTcpDemoDlg* pThis = (CTcpDemoDlg*)pParam;
	return pThis->Thread_Listen();
}

int CTcpDemoDlg::Thread_Listen(void)
{
	int CmdID = -1;
	int Status	= -1;
	int Result = -1;
	CString ErrInfo;
	CString TestData;

	CString szTemp;
	while(m_bAcceptConnectLoop)
	{
		if (m_tcpServer.m_bConnected == FALSE)
		{
			ShowSendMsg.SetWindowText(L"�����ѶϿ��������½������ӣ�");
			Sleep(500);
			continue;
		}

		if (RecvMsg(CmdID, Status, Result, ErrInfo, TestData) == FALSE)
		{
			Sleep(500);
			continue;
		}
		else
		{
		
		}
	}


	return 0;
}

UINT CTcpDemoDlg::Thread_ListenClient(LPVOID pParam)
{
	CTcpDemoDlg* pThis = (CTcpDemoDlg*)pParam;
	return pThis->Thread_ListenClient();
}

int CTcpDemoDlg::Thread_ListenClient(void)
{
	int CmdID = -1;
	int Status = -1;
	int Result = -1;
	CString ErrInfo;
	CString TestData;

	CString szTemp;
	while (m_bAcceptConnectLoop)
	{
		if (m_tcpClient.m_bConnected == FALSE)
		{
			ShowSendMsg.SetWindowText(L"�����ѶϿ��������½������ӣ�");
			Sleep(500);
			continue;
		}

		if (RecvMsgClient(CmdID, Status, Result, ErrInfo, TestData) == FALSE)
		{
			Sleep(500);
			continue;
		}
		else
		{
		}
	}


	return 0;
}

BOOL CTcpDemoDlg::SendMsgClient(int CmdID, int Status, int Result, CString ErrInfo, CString TestData)
{
	BOOL bRet = FALSE;
	SockMsg* pMsg = new SockMsg;
	pMsg->CmdID = CmdID;
	pMsg->Status = Status;
	pMsg->Result = Result;

	UnicodeToAnsi(ErrInfo.GetBuffer(), pMsg->ErrInfo, 256);
	ErrInfo.ReleaseBuffer();

	UnicodeToAnsi(TestData.GetBuffer(), pMsg->TestData, 256);
	TestData.ReleaseBuffer();

	char SendBuf[1024];
	int len = sizeof(SockMsg);
	memcpy(SendBuf, pMsg, len);
	CString szTemp;
	szTemp.Format(_T("Send:%d/%d/%d/%s/%s"), CmdID, Status, Result, ErrInfo, TestData);
	int nRet = m_tcpClient.Send(SendBuf, len);
	if (nRet == SE_OK)
	{
		ShowSendMsg.SetWindowText(szTemp);
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}


	return bRet;
}

BOOL CTcpDemoDlg::SendMsgClient(SockMsg* pMsg)
{
	BOOL bRet = FALSE;

	char SendBuf[1024];
	int len = sizeof(SockMsg);
	memcpy(SendBuf, pMsg, len);
	CString szTemp;
	szTemp.Format(_T("Send:%d/%d/%d/%s/%s"), CmdID, Status, Result, ErrInfo, TestData);
	int nRet = m_tcpClient.Send(SendBuf, len);
	if (nRet == SE_OK)
	{
		ShowSendMsg.SetWindowText(szTemp);
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}


	return bRet;
}

BOOL CTcpDemoDlg::RecvMsgClient(int& CmdID, int& Status, int& Result, CString& ErrInfo, CString& TestData)
{
	BOOL bRet = FALSE;

	SockMsg pMsg;
	char recvData[1024];
	int recvLen = 0;
	int len = sizeof(SockMsg);
	int nRet = m_tcpClient.Recieve(recvData, len, recvLen);
	if (nRet == SE_OK)
	{
		memcpy(&pMsg, recvData, recvLen);
		CmdID = pMsg.CmdID;
		Status = pMsg.Status;
		Result = pMsg.Result;
		ErrInfo = CString(AnsiToUnicode(pMsg.ErrInfo));
		TestData = CString(AnsiToUnicode(pMsg.TestData));

		CString szTemp;
		szTemp.Format(_T("Recv:%d/%d/%d/%s/%s"), CmdID, Status, Result, ErrInfo, TestData);
		ShowRecvMsg.SetWindowTextW(szTemp);
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

BOOL CTcpDemoDlg::SendMsg(int CmdID, int Status, int Result, CString ErrInfo, CString TestData)
{
	BOOL bRet = FALSE;
	SockMsg* pMsg = new SockMsg;
	pMsg->CmdID = CmdID;
	pMsg->Status = Status;
	pMsg->Result = Result;

	UnicodeToAnsi(ErrInfo.GetBuffer(),pMsg->ErrInfo, 256);
	ErrInfo.ReleaseBuffer();

	UnicodeToAnsi(TestData.GetBuffer(), pMsg->TestData, 256);
	TestData.ReleaseBuffer();

	char SendBuf[1024];
	int len = sizeof(SockMsg);
	memcpy(SendBuf, pMsg, len);
	CString szTemp;
	szTemp.Format(_T("Send:%d/%d/%d/%s/%s"), CmdID, Status, Result, ErrInfo, TestData);
	int nRet = m_tcpServer.Send(m_sock, SendBuf, len);
	if (nRet == SE_OK)
	{
		ShowSendMsg.SetWindowText(szTemp);
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}


	return bRet;
}

BOOL CTcpDemoDlg::RecvMsg(int& CmdID, int& Status, int& Result, CString& ErrInfo, CString& TestData)
{
	BOOL bRet = FALSE;

	SockMsg pMsg;
	char recvData[1024];
	int recvLen = 0;
	int len = sizeof(SockMsg);
	int nRet = m_tcpServer.Recieve(m_sock, recvData, len, recvLen);

	if (nRet == SE_OK)
	{
		memcpy(&pMsg, recvData, recvLen);
		CmdID = pMsg.CmdID;
		Status = pMsg.Status;
		Result = pMsg.Result;
		ErrInfo = CString(AnsiToUnicode(pMsg.ErrInfo));
		TestData = CString(AnsiToUnicode(pMsg.TestData));
		
		CString szTemp;
		szTemp.Format(_T("Recv:%d/%d/%d/%s/%s"), CmdID, Status, Result, ErrInfo, TestData);
		ShowRecvMsg.SetWindowTextW(szTemp);
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

void CTcpDemoDlg::OnBnClickedRadioServer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_tcpMode = TM_Server_CameraTest;
}


void CTcpDemoDlg::OnBnClickedRadioClient()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_tcpMode = TM_Client_CameraTest;
}


void CTcpDemoDlg::OnBnClickedRadioClientLensAa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_tcpMode = TM_Client_Lens;
}


void CTcpDemoDlg::OnCbnSelchangeComboAxis()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lensAA.m_axis = m_combAxis.GetCurSel() + 8;
}


void CTcpDemoDlg::OnBnClickedBtnHome()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	CString szTemp;
// 	GetDlgItemText(IDC_BTN_HOME, szTemp);
// 	if (szTemp == L"Home")
// 	{
// 		m_lensAA.SendDword(6, m_lensAA.EnableBit(0, m_lensAA.m_axis));// enable
// 	}
// 	else
// 	{
// 		m_lensAA.SendDword(6, m_lensAA.DisableBit(0, m_lensAA.m_axis));// disable
// 	}

	m_lensAA.SendDword(6, m_lensAA.EnableBit(0, m_lensAA.m_axis));
}


void CTcpDemoDlg::OnBnClickedBtnJogP()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lensAA.SendDword(2, 1 << m_lensAA.m_axis);
}


void CTcpDemoDlg::OnBnClickedBtnJogN()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lensAA.SendDword(4, 1 << m_lensAA.m_axis);
}


void CTcpDemoDlg::OnBnClickedBtnEnable()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szTemp;
	GetDlgItemText(IDC_BTN_ENABLE, szTemp);
	int Enable = m_lensAA.RecvDword(100);  //��ʹ��״̬
	if (szTemp == L"Disabled")
	{
		m_lensAA.SendDword(0, m_lensAA.EnableBit(Enable, m_lensAA.m_axis));// enable
	}
	else
	{
		m_lensAA.SendDword(0, m_lensAA.DisableBit(Enable, m_lensAA.m_axis));// disable
	}
}


void CTcpDemoDlg::OnBnClickedBtnAbsMove()
{
	m_lensAA.SendDword(8, 0 << m_lensAA.m_axis);
	CString szTemp;
	GetDlgItemText(IDC_EDIT_TARGET_POS, szTemp);
	m_lensAA.DoubleSend(384, m_lensAA.m_axis, _ttoi(szTemp), 32);
	m_lensAA.SendDword(10, 1 << m_lensAA.m_axis);
}


void CTcpDemoDlg::OnBnClickedBtnRelMove()
{
	m_lensAA.SendDword(8, 1 << m_lensAA.m_axis);
	CString szTemp;
	GetDlgItemText(IDC_EDIT_TARGET_POS, szTemp);
	m_lensAA.DoubleSend(384, m_lensAA.m_axis, _ttoi(szTemp), 32);

	m_lensAA.SendDword(10, 1 << m_lensAA.m_axis);
}


void CTcpDemoDlg::OnBnClickedBtnStop()
{
	m_lensAA.SendDword(12, 1 << m_lensAA.m_axis);
}


void CTcpDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
	{
		//��ȡPLC��IO״̬����ȡ�׵�ַΪ %W110��һ�ζ�ȡ���е�IO ����64�㣻���64��
		unsigned char* RecData = m_lensAA.ReadData(110, 8, 0xB1);
		int result[4] = { 0 };

		for (short i = 0; i < 4; i++)
		{
			result[i] = m_lensAA.ToInt32(RecData, i * 4);
		}
// 		Input1 = result[0];        //IO״̬��ʾ������
// 		Input2 = result[1];
// 		Output1 = result[2];
// 		Output2 = result[3];
		break;
	}
	case 2:
	{
		// BtnErrorStatus
		int nError = m_lensAA.RecvDword(108);
		if (nError == 1)
		{
			// red
		}
		else
		{
			// Lime
		}

 		unsigned char* RecData = m_lensAA.ReadData(200, 64, 0x81);
		for (int i = 0; i < 32; i++)
		{
			m_lensAA.m_data.AlarmData[i] = m_lensAA.ToInt32(RecData, i * 4);
		}
		for (int j = 0; j < 32; j++)
		{
			for (int i = 0; i < 32; i++)
			{
				int a = (m_lensAA.m_data.AlarmData[j] >> i) & 1;
				if (a == 0)
				{
					m_lensAA.m_data.AlarmID[i + j * 32] = 0;

				}
				else if (a == 1)
				{
					m_lensAA.m_data.AlarmID[i + j * 32] = 1;
				}
			}
		}
		//return m_lensAA.m_data.AlarmID;    //���ر���ID������
	}
	break;
	case 3:
	{
		// show tcp status
		// 
		m_lensAA.m_data.Enable = m_lensAA.RecvDword(100);  //��ʹ��״̬
		m_lensAA.m_data.Home = m_lensAA.RecvDword(102);    //�����״̬
		m_lensAA.m_data.Idle = m_lensAA.RecvDword(104);    //���˶�״̬
		m_lensAA.m_data.RunStatus = m_lensAA.RecvDword(106);   //ģʽ����״̬
		m_lensAA.m_data.LP = m_lensAA.RecvDword(122);  //����λ״̬
		m_lensAA.m_data.LN = m_lensAA.RecvDword(124);  //����λ״̬

		unsigned char* RecData = m_lensAA.ReadData(2100, 12, 0x82); // ģ����
		float n = m_lensAA.ToInt32(RecData, 3 * 4) / 10000.0f; // n[3]
// 		WIN_Q.Col_ActAnlog1.Text = Convert.ToString(n[0]);        //ǰ����ʵʱģ������ʾ 
// 		WIN_Q.Col_ActAnlog2.Text = Convert.ToString(n[1]);        //ǰ����ʵʱģ������ʾ 
// 		WIN_Q.Col_ActAnlog3.Text = Convert.ToString(n[2]);        //ǰ����ʵʱģ������ʾ 
// 		WIN_G1.Col_ActAnlog4.Text = Convert.ToString(n[3]);        //��λһʵʱģ������ʾ
// 		WIN_G2.Col_ActAnlog5.Text = Convert.ToString(n[4]);        //��λ��ʵʱģ������ʾ 
// 		WIN_H.Col_ActAnlog6.Text = Convert.ToString(n[5]);        //������ʵʱģ������ʾ 

		CString szTemp;
		if (m_lensAA.GetBit(m_lensAA.m_data.Enable, m_lensAA.m_axis))
		{
			SetDlgItemText(IDC_BTN_ENABLE, L"Enabled");
		}
		else
		{
			SetDlgItemText(IDC_BTN_ENABLE, L"Disabled");
		}

		if (m_lensAA.GetBit(m_lensAA.m_data.Home, m_lensAA.m_axis))
		{
			SetDlgItemText(IDC_BTN_HOME, L"Homed");
		}
		else
		{
			SetDlgItemText(IDC_BTN_HOME, L"Home");
		}

		if (m_lensAA.GetBit(m_lensAA.m_data.LP, m_lensAA.m_axis))
		{
			SetDlgItemText(IDC_STATIC_P_LIMIT, L"O");
		}
		else
		{
			SetDlgItemText(IDC_STATIC_P_LIMIT, L"X");
		}

		if (m_lensAA.GetBit(m_lensAA.m_data.LN, m_lensAA.m_axis))
		{
			SetDlgItemText(IDC_STATIC_N_LIMIT, L"O");
		}
		else
		{
			SetDlgItemText(IDC_STATIC_N_LIMIT, L"X");
		}

		if (m_lensAA.GetBit(m_lensAA.m_data.Idle, m_lensAA.m_axis))
		{
			SetDlgItemText(IDC_STATIC_IDEL, L"idel");
		}
		else
		{
			SetDlgItemText(IDC_STATIC_IDEL, L"busy");
		}
	}
	break;
	case 4:
	{
		//��ȡPLC��ĵ�ǰλ�ã���ȡ�׵�ַΪ %D2000��һ�ζ�ȡ32��
		unsigned char* RecData = m_lensAA.ReadData(2000, 64, 0x82);
		float actPos = m_lensAA.ToInt32(RecData, m_lensAA.m_axis*4)/10000.0f;
		CString szTemp;
		szTemp.Format(L"%f", actPos);
		SetDlgItemText(IDC_STATIC_ACTUAL_POS, szTemp);
		SAFE_DELETE_ARRAY(RecData);
	}
	break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CTcpDemoDlg::SetMsg(SockMsg* pMsg, int CmdID, int Status, int Result, string ErrInfo, string TestData, std::vector<SockMsg>& ListSockMsg)
{
	pMsg->CmdID = CmdID;
	pMsg->Status = Status;
	pMsg->Result = Result;
	// 	pMsg->ErrInfo = ErrInfo.c_str();
	// 	pMsg->TestData = TestData.c_str();
	memcpy_s(pMsg->ErrInfo, 256, ErrInfo.c_str(), ErrInfo.length() + 1);
	memcpy_s(pMsg->TestData, 256, TestData.c_str(), TestData.length() + 1);


	ListSockMsg.push_back(*pMsg);
}

void CTcpDemoDlg::WriteListSockMsg(std::vector<SockMsg>& listSockMsg)
{
	pugi::xml_document xdoc;
	pugi::xml_node xdec = xdoc.prepend_child(pugi::node_declaration);
	xdec.append_attribute("version").set_value("1.0");
	xdec.append_attribute("encoding").set_value("utf-8");

	pugi::xml_node xstudents = xdoc.append_child("ListSockMsg");
	xstudents.append_child(pugi::node_comment).set_value("ListSockMsg Comment");
	for (const SockMsg& msg : listSockMsg)
	{
		pugi::xml_node xstudent = xstudents.append_child("MsgName");

		// add attribute
		pugi::xml_attribute xstudent_TestData = xstudent.append_attribute("TestData");
		xstudent_TestData.set_value(msg.TestData);
		pugi::xml_attribute xstudent_ErrInfo = xstudent.append_attribute("ErrInfo");
		xstudent_ErrInfo.set_value(msg.ErrInfo);
		pugi::xml_attribute xstudent_Result = xstudent.append_attribute("Result");
		xstudent_Result.set_value(msg.Result);
		pugi::xml_attribute xstudent_Status = xstudent.append_attribute("Status");
		xstudent_Status.set_value(msg.Status);
		pugi::xml_attribute xstudent_CmdID = xstudent.append_attribute("CmdID");
		xstudent_CmdID.set_value(msg.CmdID);

		// add child
		// 		char temp[256] = { 0 };
		// 		pugi::xml_node xname = xstudents.append_child("CmdID");
		// 		sprintf_s(temp, 256, "%d", msg.CmdID);
		// 		xname.append_child(pugi::node_pcdata).set_value(temp);
		// 		xname = xstudents.append_child("Status");
		// 		sprintf_s(temp, 256, "%d", msg.Status);
		// 		xname.append_child(pugi::node_pcdata).set_value(temp);
		// 		xname = xstudents.append_child("Result");
		// 		sprintf_s(temp, 256, "%d", msg.Status);
		// 		xname.append_child(pugi::node_pcdata).set_value(temp);
		// 		xname = xstudents.append_child("ErrInfo");
		// 		xname.append_child(pugi::node_pcdata).set_value(msg.ErrInfo);
		// 		xname = xstudents.append_child("TestData");
		// 		xname.append_child(pugi::node_pcdata).set_value(msg.TestData);

	}
//	xdoc.print(std::cout);
	xdoc.save_file(kFileName.GetString());
}

void CTcpDemoDlg::ReadListSockMsg(std::vector<SockMsg>& listSockMsg)
{
	listSockMsg.clear();
	CString kFileName = GetExeDir() + L"ListSockMsg.xml";
	pugi::xml_document xdoc;
	if (!xdoc.load_file(kFileName.GetString()))
	{
		return;
	}

	pugi::xml_node xstudents = xdoc.child("ListSockMsg");
	for (pugi::xml_node xstudent = xstudents.first_child(); xstudent != NULL; xstudent = xstudent.next_sibling())
	{
		SockMsg tempMsg;
	//	InitMsg(&tempMsg);
		tempMsg.CmdID = xstudent.attribute("CmdID").as_int();

		tempMsg.Status = xstudent.attribute("Status").as_int();

		tempMsg.Result = xstudent.attribute("Result").as_int();

		string szErrorInfo = xstudent.attribute("ErrInfo").as_string();
		memcpy_s(tempMsg.ErrInfo, 256, szErrorInfo.c_str(), szErrorInfo.length() + 1);

		string szTestData = xstudent.attribute("TestData").as_string();
		memcpy_s(tempMsg.TestData, 256, szTestData.c_str(), szTestData.length() + 1);

		listSockMsg.push_back(tempMsg);
	}

	for (const SockMsg& msg : listSockMsg)
	{
	//	std::cout << msg.CmdID << "/" << msg.Status << "/" << msg.Result << "/" << msg.ErrInfo << "/" << msg.TestData << std::endl;
		
	}
}

void CTcpDemoDlg::OnBnClickedBtnAutoStart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ReadListSockMsg(m_ListSockMsg);

	for (int i = 0; i < m_ListSockMsg.size(); i++)
	{
		SockMsg* pMsg = &m_ListSockMsg.at(i);
		SendMsg(pMsg->CmdID, pMsg->Status, pMsg->Result, CString(pMsg->ErrInfo), CString(pMsg->TestData));
		Sleep(5000);
	}
	
}

void CTcpDemoDlg::InitMsg(SockMsg* pMsg)
{
	pMsg->CmdID = 1;
	pMsg->Status = -1;
	pMsg->Result = -1;
	memset(pMsg->ErrInfo, 0, 256);
	memset(pMsg->TestData, 0, 256);
}