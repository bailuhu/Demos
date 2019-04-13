
// TcpDemoDlg.h : 头文件
//

#pragma once

#include "TcpServerManager.h"
#include "TcpClientManager.h"
#include "LensAA.h"

#include "pugixml.hpp"
#include <vector>

enum TCP_MODE
{
	TM_Server_CameraTest,
	TM_Client_CameraTest,
	TM_Client_Lens,
	
};


// CTcpDemoDlg 对话框
class CTcpDemoDlg : public CDialogEx
{
// 构造
public:
	CTcpDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TCPDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioServer();
	afx_msg void OnBnClickedRadioClient();
	DECLARE_MESSAGE_MAP()
public:

	int CmdID;
	int Status;
	int Result;
	CString ErrInfo;
	CString TestData;
	CStatic ShowSendMsg;
	CStatic ShowRecvMsg;
	
	int m_tcpMode;

	TcpServerManager m_tcpServer;
	TcpClientManager m_tcpClient;
	BOOL m_bAcceptConnectLoop;
	SOCKET m_sock;

	BOOL SendMsg(int CmdID, int Status, int Result, CString ErrInfo, CString TestData);
	BOOL RecvMsg(int& CmdID, int& Status, int& Result, CString& ErrInfo, CString& TestData);
	BOOL SendMsgClient(int CmdID, int Status, int Result, CString ErrInfo, CString TestData);
	BOOL SendMsgClient(SockMsg* pMsg);
	BOOL RecvMsgClient(int& CmdID, int& Status, int& Result, CString& ErrInfo, CString& TestData);

	static UINT Thread_AcceptConnect(LPVOID pParam);
	int Thread_AcceptConnect(void);

	static UINT Thread_Listen(LPVOID pParam);
	int Thread_Listen(void);
	static UINT Thread_ListenClient(LPVOID pParam);
	int Thread_ListenClient(void);

	double m_jogVel;
	LensAA m_lensAA;
	afx_msg void OnBnClickedRadioClientLensAa();
	afx_msg void OnCbnSelchangeComboAxis();
	CComboBox m_combAxis;
	afx_msg void OnBnClickedBtnHome();
	afx_msg void OnBnClickedBtnJogP();
	afx_msg void OnBnClickedBtnJogN();
	afx_msg void OnBnClickedBtnEnable();
	afx_msg void OnBnClickedBtnAbsMove();
	afx_msg void OnBnClickedBtnRelMove();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	// 
	CString kFileName;
	vector<SockMsg> m_ListSockMsg;
	void InitMsg(SockMsg* pMsg);// 初始化结构体Msg、清零
	void SetMsg(SockMsg* pMsg, int CmdID, int Status, int Result, string ErrInfo, string TestData, std::vector<SockMsg>& ListSockMsg);
	void WriteListSockMsg(std::vector<SockMsg>& listSockMsg);
	void ReadListSockMsg(std::vector<SockMsg>& listSockMsg);
	afx_msg void OnBnClickedBtnAutoStart();
};
