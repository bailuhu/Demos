#include "stdafx.h"

#include "TcpServerManager.h"

TcpServerManager::TcpServerManager(void)
{
	m_Sock = INVALID_SOCKET;
	m_bInitialized = FALSE;
}

TcpServerManager::~TcpServerManager(void)
{
	CloseSocket();
}


BOOL TcpServerManager::InitWSA()
{
	enum
	{
		DeputyVerNum = 1,
		MainVerNum = 2,
	};

	WORD wVersionRequired;
	WSADATA wsaData;
	wVersionRequired = MAKEWORD(DeputyVerNum, MainVerNum);

	int nRet = WSAStartup(wVersionRequired, &wsaData);

	if (nRet == 0)
		return TRUE;
	else
		ErrorLog(nRet);

	if (HIBYTE(wsaData.wVersion) != DeputyVerNum ||
		LOBYTE(wsaData.wVersion) != MainVerNum)
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL TcpServerManager::CloseSocket()
{
	if (closesocket(m_Sock) == SOCKET_ERROR)
	{
		ErrorLog(WSAGetLastError());		
	}

	m_bInitialized = FALSE;

	return TRUE;
}

BOOL TcpServerManager::ReleaseWSA()
{
	if (WSACleanup() == SOCKET_ERROR)
	{
		ErrorLog(WSAGetLastError());	
	}



	return TRUE;
}

BOOL TcpServerManager::InitSocket(USHORT BindPort, wstring IP)
{
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_Sock == INVALID_SOCKET)	return FALSE;

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(BindPort);
	//addr.sin_addr.S_un.S_addr = GetHostNetIP();

	if (IP == _T(""))
	{
		addr.sin_addr.S_un.S_addr = GetHostNetIP();//htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.S_un.S_addr = wstingIpToInetIP(IP);  //htonl(INADDR_ANY);
	}

	int nRet = bind(m_Sock, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	if (nRet == SOCKET_ERROR ) return FALSE;

	nRet = listen(m_Sock, 10);
	if (nRet == SOCKET_ERROR ) return FALSE;

	m_BindPort = BindPort;

	//取消延时
	int on = 1;  
	setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(on));  

	return TRUE;
}

int TcpServerManager::Init(USHORT BindPort, wstring IP)
{
	m_bInitialized = FALSE;

	if (!InitWSA()) return SE_INIT_FAIL;

	if(!InitSocket(BindPort, IP)) 
	{
		Close();
		return SE_INIT_FAIL;
	}

	m_bInitialized = TRUE;
	return SE_OK;
}


int TcpServerManager::Init(CString szPort, CString szIP)
{
	m_bInitialized = FALSE;

	if (!InitWSA()) return SE_INIT_FAIL;

	// GetBuffer()主要作用是将字符串的缓冲区长度锁定，releaseBuffer则是解除锁定，
	// 使得CString对象在以后的代码中继续可以实现长度自适应增长的功能。
	wstring wszTemp(szIP.GetBuffer());
	szIP.ReleaseBuffer();
	if(!InitSocket(StrToInt(szPort), wszTemp))
	{
		Close();
		return SE_INIT_FAIL;
	}

	m_bInitialized = TRUE;
	return SE_OK;
}

void TcpServerManager::Close()
{
	CloseSocket();
	m_bInitialized = FALSE;
}

int TcpServerManager::SetSocketOption(UINT bufsize, UINT SendTimeOut, UINT RecvTimeOut)
{
	if (bufsize > 0)
	{
		setsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(int) );
		setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(int) );
	}

	if (SendTimeOut > 0)	
	{
		setsockopt(m_Sock, SOL_SOCKET, SO_SNDTIMEO,(const char*)&SendTimeOut, sizeof(int) );
	}

	if (RecvTimeOut > 0)	
	{
		setsockopt(m_Sock, SOL_SOCKET, SO_RCVTIMEO,(const char*)&RecvTimeOut, sizeof(int) ); 	
	}

	return SE_OK;
}


int TcpServerManager::Accept(UINT& INetIP, USHORT& INetPort, SOCKET& sockConn)
{
	SOCKADDR_IN addrClient;
	int slen = sizeof(SOCKADDR);
	sockConn = accept(m_Sock, (SOCKADDR*)&addrClient, &slen);
	if (sockConn == INVALID_SOCKET) return -1;

	INetIP = addrClient.sin_addr.S_un.S_addr;
	INetPort = addrClient.sin_port;

	return SE_OK;
}

int TcpServerManager::Accept(wstring& IP, USHORT& port, SOCKET& sockConn)
{
	SOCKADDR_IN addrClient;
	int slen = sizeof(SOCKADDR);
	sockConn = accept(m_Sock, (SOCKADDR*)&addrClient, &slen);
	if (sockConn == INVALID_SOCKET) return -1;

	IP = INetIPtoWSzIP(addrClient.sin_addr.S_un.S_addr);
	port = ntohs(addrClient.sin_port);

	return SE_OK;
}

int TcpServerManager::Accept(wstring& IP, wstring& Port, SOCKET& sockConn)
{
	SOCKADDR_IN addrClient;
	int slen = sizeof(SOCKADDR);
	sockConn = accept(m_Sock, (SOCKADDR*)&addrClient, &slen);
	if (sockConn == INVALID_SOCKET) return -1;

	IP = INetIPtoWSzIP(addrClient.sin_addr.S_un.S_addr);

	wchar_t temp[8] = {0};
	wsprintf(temp, L"%hd", ntohs(addrClient.sin_port));
	Port = temp;

	return SE_OK;
}

int TcpServerManager::Send(SOCKET sockConn, wstring szData)
{
	if (sockConn == INVALID_SOCKET) return SE_FAIL_TO_CONNECT;

	char* Data = UnicodeToUtf8((wchar_t*)szData.c_str());//转换为utf-8

	int length = (int)strlen(Data);

	int val = send(sockConn, Data, length, 0);

	delete [] Data;

	if (val != length)		return SE_SEND_FAIL;

	return SE_OK;
}

int TcpServerManager::Send(SOCKET sockConn, char* Data, int length)
{
	if (sockConn == INVALID_SOCKET) return SE_FAIL_TO_CONNECT;

	int val = send(sockConn, Data, length, 0);

	if (val != length)		return SE_SEND_FAIL;

	return SE_OK;
}

int TcpServerManager::Recieve(SOCKET sockConn, wstring& Data)
{
	if (sockConn == INVALID_SOCKET) return SE_FAIL_TO_CONNECT;

	const int bufsize = 1024*64; //64K
	char buf[bufsize] = {0};

	int Cnt = recv(sockConn, buf, bufsize, 0);

	if (Cnt == 0) return SE_SOCKET_DISCONNECT;
	if (Cnt < 0 ) 	return SE_RECV_FAIL;

	wchar_t* p = Utf8ToUnicode(buf);
	Data = p;

	delete [] p;

	return SE_OK;
}

int TcpServerManager::Recieve( SOCKET sockConn, char* Data, int len, int& ReadLen)
{
	if (sockConn == INVALID_SOCKET) return SE_SOCKET_ERROR;

	int Cnt = recv(sockConn, Data, len, 0);

	if (Cnt == 0) return SE_SOCKET_DISCONNECT;
	if (Cnt < 0 ) 	return SE_RECV_FAIL;

	ReadLen = Cnt;

	return SE_OK;
}

int TcpServerManager::SetNonBlockMode(BOOL bNon)
{
	unsigned long ul = 1;
	if (bNon)
		ul = 1; //非阻塞
	else
		ul = 0;

	int nRet = ioctlsocket(m_Sock, FIONBIO, &ul);
	if(nRet != 0) 
		return SE_SOCKET_ERROR;
	else
		return SE_OK;
}



int TcpServerManager::Accept_withTimeOut(wstring& IP, wstring& Port, SOCKET& sockConn, int wTime)
{
	//设置超时
	timeval Time;
	Time.tv_sec = wTime/1000; //连接超时
	Time.tv_usec = wTime - 1000*Time.tv_sec;

	fd_set r;
	FD_ZERO(&r);
	FD_SET(m_Sock, &r);
	//判断返回值
	int nRet = select(0, &r, 0, 0, &Time);

	if (nRet > 0) Accept(IP, Port, sockConn);

	if ( nRet <= 0 )  	return SE_FAIL_TO_ACCEPT;

	return SE_OK;
}
