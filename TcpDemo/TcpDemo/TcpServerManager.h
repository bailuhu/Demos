#pragma once

#include "GlobalSocket.h"

class TcpServerManager
{
public:
	TcpServerManager(void);
	~TcpServerManager(void);

	// Tcp��ʼ��ʱ���˿�һ��Ҫָ����IP�п����Ǳ���������Port��ǰ��IP�ں�
	int Init(USHORT  BindPort, wstring IP = _T("")); //1 �˿�
	int Init(CString szPort, CString szIP);

	int Accept(wstring& IP, wstring& Port, SOCKET& sockConn);
	int Accept(wstring& IP, USHORT& port, SOCKET& sockConn);
	int Accept(UINT& INetIP, USHORT& INetPort, SOCKET& sockConn);
	int SetSocketOption(UINT bufsize,  UINT SendTimeOut, UINT RecvTimeOut);
	void Close(); //�ر�socket �� WSA
	BOOL CloseSocket(); //ֻ�ر� socket;
	int Send(SOCKET sockConn, wstring Data); 
	int Send(SOCKET sockConn, char* Data, int length); 

	int Recieve(SOCKET sockConn, wstring& Data);
	int Recieve(SOCKET sockConn, char* Data, int len, int& ReadLen);

	int SetNonBlockMode(BOOL bNon); //���������ͷ�����ģʽ

	int Accept_withTimeOut(wstring& IP, wstring& Port, SOCKET& sockConn, int wTime); //select wTime(����)

	BOOL m_bConnected;	// ������


private:
	BOOL InitWSA();
	BOOL InitSocket(USHORT BindPort, wstring IP = _T(""));

	BOOL ReleaseWSA();
	BOOL m_bInitialized;
	SOCKET m_Sock;	// ֻ���ڲ�ʹ�õ�Sock
	BOOL m_BindPort; //litte


};