#pragma once

#include "GlobalSocket.h"

class TcpClientManager
{
public:
	TcpClientManager(void);
	~TcpClientManager(void);

	int Init();

	int Connect(wstring IP, wstring Port);
	int Connect(wstring IP, USHORT port);
	int Connect(UINT INetIP, USHORT INetPort);
	int SetSocketOption(UINT bufsize, UINT SendTimeOut, UINT RecvTimeOut); 
	void Close(); //关闭socket 和 WSA
	BOOL CloseSocket(); //只关闭 socket;

	int Send(wstring Data); 
	int Send(char* Data, int length); 

	int Recieve(wstring& Data);
	//读取指定的长度
	int Recieve(char* Data, int len, int& nRecvLen);

	int SetNonBlockMode(BOOL bNon); //设置阻塞和非阻塞模式

	int Connect_withTimeOut(wstring IP, wstring Port, int wTime); //非阻塞 + selcet wTime (毫秒)

	BOOL m_bConnected;	// 已连接
	SOCKET m_sockConn;	// 收发消息的Sock

private:
	BOOL InitWSA();
	BOOL InitSocket();
	BOOL ReleaseWSA();
	BOOL m_bInitialized;
	SOCKET m_Sock;
	BOOL m_BindPort; //litte

	// set send timeout
	int SetSendTimeout(SOCKET& Sock, int Timeout);
	// set receive timeout
	int SetRecvTimeout(SOCKET& Sock, int Timeout);
	//设置接收缓冲区buf
	int SetRecBuf(SOCKET& Sock, int Bufsize); 

};

