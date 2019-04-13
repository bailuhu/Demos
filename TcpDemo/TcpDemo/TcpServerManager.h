#pragma once

#include "GlobalSocket.h"

class TcpServerManager
{
public:
	TcpServerManager(void);
	~TcpServerManager(void);

	// Tcp初始化时，端口一定要指定，IP有可能是本机，所以Port在前，IP在后
	int Init(USHORT  BindPort, wstring IP = _T("")); //1 端口
	int Init(CString szPort, CString szIP);

	int Accept(wstring& IP, wstring& Port, SOCKET& sockConn);
	int Accept(wstring& IP, USHORT& port, SOCKET& sockConn);
	int Accept(UINT& INetIP, USHORT& INetPort, SOCKET& sockConn);
	int SetSocketOption(UINT bufsize,  UINT SendTimeOut, UINT RecvTimeOut);
	void Close(); //关闭socket 和 WSA
	BOOL CloseSocket(); //只关闭 socket;
	int Send(SOCKET sockConn, wstring Data); 
	int Send(SOCKET sockConn, char* Data, int length); 

	int Recieve(SOCKET sockConn, wstring& Data);
	int Recieve(SOCKET sockConn, char* Data, int len, int& ReadLen);

	int SetNonBlockMode(BOOL bNon); //设置阻塞和非阻塞模式

	int Accept_withTimeOut(wstring& IP, wstring& Port, SOCKET& sockConn, int wTime); //select wTime(毫秒)

	BOOL m_bConnected;	// 已连接


private:
	BOOL InitWSA();
	BOOL InitSocket(USHORT BindPort, wstring IP = _T(""));

	BOOL ReleaseWSA();
	BOOL m_bInitialized;
	SOCKET m_Sock;	// 只在内部使用的Sock
	BOOL m_BindPort; //litte


};